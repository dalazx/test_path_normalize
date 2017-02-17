#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "path.h"


void test_normalize_null() {
    assert(!normalize(NULL));
}

void test_normalize_max_path_length() {
    size_t path_len = 2048;
    char path[path_len];
    memset(&path, '.', path_len);
    path[path_len - 1] = '\0';
    assert(!normalize(path));
}

void test_normalize(const char *orig_path, const char *expected_path) {
    printf("orig: %s; expected: %s\n", orig_path, expected_path);
    char *path = normalize(orig_path);
    printf("orig: %s; actual: %s; expected: %s\n",
           orig_path, path, expected_path);
    assert(!strcmp(path, expected_path));
    free(path);
}

void test_normalize_all() {
    test_normalize_null();
    test_normalize_max_path_length();

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

    test_normalize(".foo", "/.foo");
    test_normalize(".foo.", "/.foo.");
    test_normalize(".foo/.bar", "/.foo/.bar");
    test_normalize("/.foo/.bar", "/.foo/.bar");
    test_normalize("/.foo/.bar/", "/.foo/.bar/");
    test_normalize("././.foo/.bar/./.", "/.foo/.bar/");

    test_normalize("..", "/");
    test_normalize("../", "/");
    test_normalize("/..", "/");
    test_normalize("/../", "/");
    test_normalize("../..", "/");
    test_normalize("/../..", "/");
    test_normalize("../../", "/");
    test_normalize("/../../", "/");

    test_normalize("/foo/..", "/");
    test_normalize("foo/..", "/");
    test_normalize("foo/../", "/");

    test_normalize("foo/bar/..", "/foo/");
    test_normalize("foo/../bar", "/bar");
    test_normalize("foo/../bar/", "/bar/");
    test_normalize("/foo/../bar/", "/bar/");
    test_normalize("/foo/../bar/..", "/");
    test_normalize("/foo/../bar/../", "/");
    test_normalize("/foo/bar/../baz/", "/foo/baz/");
    test_normalize("/foo/../bar/../baz", "/baz");
    test_normalize("/foo/../bar/../baz/", "/baz/");

    test_normalize("/foo/./bar/./baz", "/foo/bar/baz");

    test_normalize("...", "/...");
    test_normalize("/...", "/...");
    test_normalize(".../", "/.../");
    test_normalize("/.../", "/.../");
    test_normalize(".../...", "/.../...");
    test_normalize("/.../.../", "/.../.../");
}

int main(int argc, char **argv) {
    test_normalize_all();
}
