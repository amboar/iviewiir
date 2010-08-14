#ifndef LIBIVIEW_H
#define LIBIVIEW_H

#define IV_MIN(a,b) (a <= b ? a : b)
#define IV_MAX(a,b) (a >= b ? a : b)

struct iv_rtmp_info {
    char *url;
    char *host;
    char *app;
};

struct iv_config {
    char *api;
    char *auth;
    char *tray;
    char *categories;
    char *classifications;
    char *captions;
    int captions_offset;
    unsigned short live_streaming;
    struct iv_rtmp_info server_streaming;
    char *server_fallback;
    char *highlights;
    char *home;
    char *geo;
    char *time;
    char *feedback_url;
};

ssize_t iv_get_config(char **buf_ptr);
int iv_parse_config(struct iv_config *config, const char *buf, size_t len);
void iv_destroy_config(struct iv_config *config);

#if __STDC_VERSION__ == 199901L
#define INLINE inline
#elif __GNUC__
#define INLINE extern inline
#else
#define INLINE
#endif

#endif /* LIBIVIEW_H */
