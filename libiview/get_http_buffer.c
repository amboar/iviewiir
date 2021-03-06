#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libxml/nanohttp.h>
#include <stdio.h>
#include <errno.h>

#include "iview.h"
#include "internal.h"
#ifdef GEKKO
#include "wiicode/http.h"
#endif

ssize_t iv_get_http_buffer(const char *uri, char **buf_ptr) {
#ifdef GEKKO
    int res;
    u32 status = 0, len = 0;

    IV_DEBUG("Fetching %s.\n", uri);
    res = http_request(uri, 1 << 31);
    http_get_result(&status, (u8 **)buf_ptr, &len);
    IV_DEBUG("Request %d, HTTP status %d, length %d.\n", res, status, len);
    /* Cast is to avoid an error with powerpc-eabi-gcc.  Should be safe for any
     * sane xml buffer length.*/
    return res ? (ssize_t)len : -IV_EREQUEST;
}
#else
    int return_val;
    void *ctx = xmlNanoHTTPOpen(uri, NULL);
    if(!ctx) {
        IV_DEBUG("Failed to open %s\n", uri);
        return_val = -IV_EREQUEST;
        goto http_cleanup;
    }
    IV_DEBUG("Opened %s\n", uri);
    int http_code = -1;
    if(200 != (http_code = xmlNanoHTTPReturnCode(ctx))) {
        IV_DEBUG("Bad return code: %d\n", http_code);
        return_val = -IV_EREQUEST;
        goto http_close;
    }
    // Allocate buffer for response
#define BUF_CHUNK_SIZE (4096)
    *buf_ptr = (char *)malloc(BUF_CHUNK_SIZE);
    if(!*buf_ptr) {
        return_val = -(errno);
        goto http_close;
    }
    char *new_buf;
    char *read_buf = *buf_ptr;
    unsigned short alloc_count = 1;
    ssize_t read_len = 0;
    size_t total_len = 0;
    // Read response into buffer
    while(BUF_CHUNK_SIZE ==
            (read_len = xmlNanoHTTPRead(ctx, read_buf, BUF_CHUNK_SIZE))) {
        if(!(new_buf = realloc(*buf_ptr, ++alloc_count*BUF_CHUNK_SIZE))) {
            return_val = -(errno);
            goto alloc_error;
        }
        *buf_ptr = new_buf;
        total_len += read_len;
        read_buf = *buf_ptr + total_len;
    }
    if(-1 == read_len) {
        return_val = -IV_EREQUEST;
        goto read_error;
    }
    total_len += read_len;
    // Trim buffer to size
    if(!(new_buf = realloc(*buf_ptr, total_len+1))) {
        return_val = -(errno);
        goto alloc_error;
    }
    *buf_ptr = new_buf;
    (*buf_ptr)[total_len] = '\0';
    IV_DEBUG("HTTP read complete\n");
    return_val = total_len;
http_close:
    xmlNanoHTTPClose(ctx);
http_cleanup:
    xmlNanoHTTPCleanup();
    return return_val;
read_error:
alloc_error:
    free(*buf_ptr);
    *buf_ptr = NULL;
    goto http_close;
}
#undef BUF_CHUNK_SIZE
#endif /* GEKKO */
