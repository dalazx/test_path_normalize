#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


char *normalize(const char *path) {
    if (!path) {
        return NULL;
    }

    size_t path_len = strlen(path);
    // TODO: check some reasonable boundaries
    // printf("length: %zd\n", path_len);

    size_t path_buf_len = path_len + 2; // root slash + null byte
    char path_buf[path_buf_len];
    char *path_buf_ptr = path_buf + path_buf_len;
    *--path_buf_ptr = '\0';

    size_t skip_counter = 0;
    enum state_t {NOTHING, SEGMENT, SLASH, ONE_DOT, TWO_DOTS} state = NOTHING;
    for (int i = path_len - 1; i >= 0; i--) {
        char c = path[i];
        switch (state) {
            case NOTHING:
                state = SEGMENT;
            case SEGMENT:
                if (c == '/') {
                    state = SLASH;
                    continue;
                } else if (c == '.') {
                    state = ONE_DOT;
                    continue;
                }
                break;
            case SLASH:
                if (c == '/') {
                    continue;
                } else if (c == '.') {
                    state = ONE_DOT;
                    continue;
                } else {
                    state = SEGMENT;
                    *--path_buf_ptr = '/';
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
                    *--path_buf_ptr = '.';
                }
                break;
            case TWO_DOTS:
                if (c == '/') {
                    // skip
                } else if (c == '.') {
                    // comment
                    continue;
                } else {
                    state = SEGMENT;
                    *--path_buf_ptr = '.';
                    *--path_buf_ptr = '.';
                }
                break;
        }
        *--path_buf_ptr = path[i];
    }

    *--path_buf_ptr = '/';

    return strdup(path_buf_ptr);
}

void test_normalize(const char *orig_path, const char *expected_path) {
    char *path = normalize(orig_path);
    printf("orig: %s; actual: %s; expected: %s\n",
           orig_path, path, expected_path);
    assert(!strcmp(path, expected_path));
    free(path);
}

void test_base() {
    normalize(NULL);
    test_normalize("/", "/");
    test_normalize("//", "/");
    test_normalize("///", "/");

    test_normalize(".", "/");
    test_normalize("./", "/");
    test_normalize("./.", "/");
    test_normalize("/././", "/");
    test_normalize("./././", "/");
    test_normalize("./././.", "/");

    test_normalize("foo", "/foo");
    test_normalize("/foo", "/foo");
    test_normalize("./foo", "/foo");
    test_normalize("/foo/", "/foo/");
    test_normalize("./foo/", "/foo/");
    test_normalize("./foo/.", "/foo/");

    test_normalize("foo/bar", "/foo/bar");
    test_normalize("/foo/bar", "/foo/bar");
    test_normalize("/foo/bar/", "/foo/bar/");
    test_normalize("/foo/bar/.", "/foo/bar/");
    test_normalize("./foo/bar/./.", "/foo/bar/");

    // hidden files
}

int main(int argc, char **argv) {
    test_base();
}
