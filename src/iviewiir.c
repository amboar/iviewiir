#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <neon/ne_uri.h>
#include "iviewiir.h"
#include "libiview/iview.h"

void iviewiir_configure(struct iv_config *config) {
    char *config_buf;
    ne_uri config_uri;
    if(ne_uri_parse(IV_CONFIG_URI, &config_uri)) {
        printf("error: uri parsing failed on %s\n", IV_CONFIG_URI);
    }
    size_t config_buf_len = iv_get_xml_buffer(&config_uri, &config_buf);
    int result = iv_parse_config(config, config_buf, config_buf_len);
    iv_destroy_xml_buffer(config_buf);
}

void iviewiir_index(struct iv_config *config) {
    char *index_xml_buf;
    struct iv_index *index;
    ssize_t len = iv_get_index(config, &index_xml_buf);
    printf("index:\n%s\n", index_xml_buf);
    printf("info: parse result = %zd\n",
            iv_parse_index(index_xml_buf, len, &index));
    iv_destroy_xml_buffer(index_xml_buf);
    iv_destroy_index(index);
}

int main(int argc, char **argv) {
    struct iv_config config;
    iviewiir_configure(&config);
    iviewiir_index(&config);
    return 0;
}
