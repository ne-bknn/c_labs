#include "vendor/unity.h"
#include "../src/lib.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_1(void) {
	TEST_ASSERT_TRUE(25 == 25);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_1);
	return UNITY_END();
}
