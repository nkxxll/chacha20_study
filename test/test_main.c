#include "munit.h"
#include "test_chacha.h"

MunitTest tests[] = {{
                         "/test-one-round",      /* name */
                         test_one_round,         /* test */
                         test_one_round_setup,   /* setup */
                         test_tear_down,         /* tear_down */
                         MUNIT_TEST_OPTION_NONE, /* options */
                         NULL                    /* parameters */
                     },
                     {
                         "/test-add-state",      /* name */
                         test_add_state,         /* test */
                         test_add_state_setup,   /* setup */
                         test_tear_down,         /* tear_down */
                         MUNIT_TEST_OPTION_NONE, /* options */
                         NULL                    /* parameters */
                     },
                     {
                         "/test-serialize",      /* name */
                         test_serialize,         /* test */
                         test_serialize_setup,   /* setup */
                         test_tear_down,         /* tear_down */
                         MUNIT_TEST_OPTION_NONE, /* options */
                         NULL                    /* parameters */
                     },
                     {
                         "/test-full-round",     /* name */
                         test_full_round,        /* test */
                         test_full_round_setup,  /* setup */
                         test_tear_down,         /* tear_down */
                         MUNIT_TEST_OPTION_NONE, /* options */
                         NULL                    /* parameters */
                     },
                     /* Mark the end of the array with an entry where the test
                      * function is NULL */
                     {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite suite = {
    "/chacha-tests",        /* name */
    tests,                  /* tests */
    NULL,                   /* suites */
    1,                      /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, const char *argv[]) {
#pragma clang diagnostic ignored                                               \
    "-Wincompatible-pointer-types-discards-qualifiers"
  return munit_suite_main(&suite, NULL, argc, argv);
}
