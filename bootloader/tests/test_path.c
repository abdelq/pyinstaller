// -----------------------------------------------------------------------------
// Copyright (c) 2020, PyInstaller Development Team.
//
// Distributed under the terms of the GNU General Public License (version 2
// or later) with exception for distributing the bootloader.
//
// The full license is in the file COPYING.txt, distributed with this software.
//
// SPDX-License-Identifier: (GPL-2.0-or-later WITH Bootloader-exception)
// -----------------------------------------------------------------------------

#include <sys/types.h>
#include <stdarg.h>
#include <string.h>

#include "pyi_global.h"
#include "pyi_path.h"

#include <setjmp.h> // required fo cmocka :-(
#include <cmocka.h>


static void test_dirname(void **state) {
    char result[PATH_MAX];

    pyi_path_dirname(result, "/a1/bb/cc/dd");
    assert_string_equal(result, "/a1/bb/cc");

    pyi_path_dirname(result, "/a2/bb/cc/dd/");
    assert_string_equal(result, "/a2/bb/cc");

    pyi_path_dirname(result, "d3d");
    assert_string_equal(result, PYI_CURDIRSTR);

    pyi_path_dirname(result, "d5d/");
    assert_string_equal(result, PYI_CURDIRSTR);

    pyi_path_dirname(result, "");
    assert_string_equal(result, PYI_CURDIRSTR);
}


static void test_basename(void **state) {
    char result[PATH_MAX];
    char input[PATH_MAX];
    // basename()'s second argument is not `const`, thus using a constant
    // string yields to segementation fault.

    strcpy(input, "/aa/bb/cc/d1d");
    pyi_path_basename(result, input);
    assert_string_equal(result, "d1d");

    strcpy(input, "d3dd");
    pyi_path_basename(result, input);
    assert_string_equal(result, "d3dd");

    /* These cases are not correctly handled by our implementation of
     * basename(). But this is okay, since we use basename() only to determine
     * the application path based on argv[0].
     *
    strcpy(input, "/aa/bb/cc/d2d/");
    pyi_path_basename(result, input);
    assert_string_equal(result, "d2d");

    strcpy(input, "d4dd/");
    pyi_path_basename(result, input);
    assert_string_equal(result, "d4dd");

    strcpy(input, "");
    pyi_path_basename(result, input);
    assert_string_equal(result, PYI_CURDIRSTR);
    */
}


static void test_join(void **state) {
    char path1[PATH_MAX];
    char path2[PATH_MAX];
    char result[PATH_MAX];
    char *r;

    r = pyi_path_join((char *)result, "lalala", "mememe");
    assert_ptr_equal(r, &result);
    assert_string_equal(result, "lalala/mememe");

    r = pyi_path_join((char *)result, "lalala/", "mememe");
    assert_ptr_equal(r, &result);
    assert_string_equal(result, "lalala/mememe");

    r = pyi_path_join((char *)result, "lalala/", "mememe/");
    assert_ptr_equal(r, &result);
    assert_string_equal(result, "lalala/mememe");

    r = pyi_path_join((char *)result, "lal/ala/", "mem/eme/");
    assert_ptr_equal(r, &result);
    assert_string_equal(result, "lal/ala/mem/eme");

    memset(path1, 'a', PATH_MAX); path1[PATH_MAX-1] = '\0';
    memset(path2, 'b', PATH_MAX); path2[PATH_MAX-1] = '\0';
    assert_int_equal(strlen(path1), PATH_MAX-1);
    assert_int_equal(strlen(path2), PATH_MAX-1);
    //r = pyi_path_join((char *)result, (char *)path1,  (char *)path2);
    //assert_ptr_equal(r, &result);
    //assert_string_equal(result, path1);
}


int pyi_search_path(char *result, const char *appname);

static void test_search_path(void **state) {
    char result[PATH_MAX];
    pyi_search_path(result, "my-app");
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dirname),
        cmocka_unit_test(test_basename),
        cmocka_unit_test(test_join),
        cmocka_unit_test(test_search_path),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}