#include "vendor/unity.h"
#include "../src/gcd_lib.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_gcd_1(void) {
	TEST_ASSERT_TRUE(gcd(125, 25) == 25);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_gcd_1);
	return UNITY_END();
}
