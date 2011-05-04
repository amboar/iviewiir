#include "iview.h"
#include "internal.h"

int iv_easy_series_items(struct iv_config *config, struct iv_series *series,
        struct iv_item **items_ptr) {
    char *series_buf = NULL;
    const ssize_t len = iv_get_series_items(config, series, &series_buf);
    if(0 >= len) {
        return len;
    }
    const ssize_t items_len = iv_parse_series_items(series_buf, len, items_ptr);
    iv_destroy_xml_buffer(series_buf);
    return items_len;
}