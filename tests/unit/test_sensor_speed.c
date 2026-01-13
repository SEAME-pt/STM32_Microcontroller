#include "unity.h"
#include "app_threadx.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_calculate_rpm(void) {

    UINT    rpm;

    rpm = convertValuesRPM(ULONG count, 
                        ULONG ticks, 
                        ULONG period,
                        t_rpm_state *state);
    
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_calculate_rpm);
    return UNITY_END();
}