#include <string.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 1024


char *normalize(const char *path) {
    if (!path) {
        return NULL;
    }

    size_t path_len = strlen(path);

    if (path_len > MAX_PATH_LENGTH) {
        return NULL;
    }

    size_t path_buf_len = path_len + 2; // root slash + null byte
    char path_buf[path_buf_len];
    char *path_buf_ptr = path_buf + path_buf_len;
    *--path_buf_ptr = '\0';

    bool segment_slash = false;
    size_t segment_end = 0;
    size_t skip_segments = 0;
    enum state_t {NOTHING, SLASH, SEGMENT, ONE_DOT, TWO_DOTS} state = NOTHING;

    for (int i = path_len - 1; i >= 0; i--) {
        char c = path[i];
        switch (state) {
            case NOTHING:
                if (c == '/') {
                    state = SLASH;
                    continue;
                } else if (c == '.') {
                    segment_end = i + 1;
                    state = ONE_DOT;
                    continue;
                } else {
                    state = SEGMENT;
                }
                break;
            case SEGMENT:
                if (!segment_slash && c == '/') {
                    state = SLASH;
                    if (skip_segments) {
                        skip_segments--;
                    }
                    continue;
                }
                segment_slash = false;
                if (skip_segments) {
                    continue;
                }
                break;
            case SLASH:
                if (c == '/') {
                    continue;
                } else if (c == '.') {
                    segment_slash = true;
                    segment_end = i + 2;
                    state = ONE_DOT;
                    continue;
                } else {
                    segment_slash = true;
                    state = SEGMENT;
                    i += 2;
                    continue;
                }
                break;
            case ONE_DOT:
                if (c == '/') {
                    state = SLASH;
                    continue;
                } else if (c == '.') {
                    state = TWO_DOTS;
                    continue;
                } else {
                    state = SEGMENT;
                    i = segment_end;
                    continue;
                }
                break;
            case TWO_DOTS:
                if (c == '/') {
                    skip_segments++;
                    state = SLASH;
                    continue;
                } else {
                    state = SEGMENT;
                    i = segment_end;
                    continue;
                }
                break;
        }
        *--path_buf_ptr = path[i];
    }

    *--path_buf_ptr = '/';

    // TODO: exit if malloc failed
    return strdup(path_buf_ptr);
}
