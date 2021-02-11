#include "vendor/unity.h"
#include "../src/lib.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_1(void) {
	struct Tuple answer = {1, 1};
	struct Tuple func_result = multiply_and_count(12);
	TEST_ASSERT_MESSAGE(tuples_eq(&func_result, &answer), "Fail at 12");
}

static void test_2(void) {
	struct Tuple answer = {0, 0};
	struct Tuple func_result = multiply_and_count(22);
	TEST_ASSERT_MESSAGE(tuples_eq(&func_result, &answer), "Fail at 22");
}

static void test_3(void) {
	struct Tuple answer = {5, 1};
	struct Tuple func_result = multiply_and_count(11111);
	TEST_ASSERT_MESSAGE(tuples_eq(&func_result, &answer), "Fail at 11111");
}

static void test_4(void) {
	struct Tuple answer = {0, 0};
	struct Tuple func_result = multiply_and_count(0);
	TEST_ASSERT_MESSAGE(tuples_eq(&func_result, &answer), "Fail at 0");
}

static void test_5(void) {
	struct Tuple answer = {1, 1};
	struct Tuple func_result = multiply_and_count(100000);
	TEST_ASSERT_MESSAGE(tuples_eq(&func_result, &answer), "Fail at 100000");
}

static void test_6(void) {
	struct Tuple answer = {4, 6561};
	struct Tuple func_result = multiply_and_count(9999);
	TEST_ASSERT_MESSAGE(tuples_eq(&func_result, &answer), "Fail at 9999");
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_1);
	RUN_TEST(test_2);
	RUN_TEST(test_3);
	RUN_TEST(test_4);
	RUN_TEST(test_5);
	RUN_TEST(test_6);
	return UNITY_END();
}
