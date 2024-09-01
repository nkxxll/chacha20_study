#ifndef TEST_CHACHA

#include "munit.h"

void *test_one_round_setup(const MunitParameter params[], void *user_data);
void test_tear_down(void *fixture);
MunitResult test_one_round(const MunitParameter params[], void *fixture);
void *test_full_round_setup(const MunitParameter params[], void *user_data);
MunitResult test_full_round(const MunitParameter params[], void *fixture);
void *test_add_state_setup(const MunitParameter params[], void *user_data);
MunitResult test_add_state(const MunitParameter params[], void *fixture);
void *test_serialize_setup(const MunitParameter params[], void *user_data);
MunitResult test_serialize(const MunitParameter params[], void *fixture);

#endif // !TEST_CHACHA
