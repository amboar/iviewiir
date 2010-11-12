#include <stdio.h>
#include <string.h>
#include <neon/ne_string.h>
#include "iview.h"

ssize_t iv_get_series_items(struct iv_config *config IV_UNUSED,
        const char *uri, struct iv_series *series, char **buf_ptr) {
    ne_uri series_uri;
    if(ne_uri_parse(uri, &series_uri)) {
        return -IV_EURIPARSE;
    }
#define QUERY_BUF_SZ 14
    char query_buf[QUERY_BUF_SZ];
    snprintf(query_buf, QUERY_BUF_SZ, "id=%d", series->id);
    series_uri.query = strdup(query_buf);
    int result = iv_get_xml_buffer(&series_uri, buf_ptr);
    ne_uri_free(&series_uri);
    return result;
}
