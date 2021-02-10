#include "vendor/unity.h"
#include "../src/lib.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_1(void) {
	struct Tuple answer = {1, 1};
	struct Tuple func_result = count_and_multiply(12);
	TEST_ASSERT_TRUE(tuples_eq(&func_result, &answer));
}

static void test_2(void) {
	struct Tuple answer = {0, 0};
	struct Tuple func_result = count_and_multiply(22);
	TEST_ASSERT_TRUE(tuples_eq(&func_result, &answer));
}

static void test_3(void) {
	struct Tuple answer = {5, 1};
	struct Tuple func_result = count_and_multiply(11111);
	TEST_ASSERT_TRUE(tuples_eq(&func_result, &answer));
}

static void test_4(void) {
	struct Tuple answer = {0, 0};
	struct Tuple func_result = count_and_multiply(0);
	TEST_ASSERT_TRUE(tuples_eq(&func_result, &answer));
}

static void test_5(void) {
	struct Tuple answer = {1, 1};
	struct Tuple func_result = count_and_multiply(100000);
	TEST_ASSERT_TRUE(tuples_eq(&func_result, &answer));
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_1);
	RUN_TEST(test_2);
	RUN_TEST(test_3);
	RUN_TEST(test_4);
	RUN_TEST(test_5);
	return UNITY_END();
}
