#include "unity.h"
#include "utils.h"

// file being tested
TEST_SOURCE_FILE("speed_rpm.c");

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

/* SWR-STM32-002 */
void test_right_calculation_of_RPM_UT_STM32_001(void) {

    UINT        rpm;
    t_rpm_state state = {0, 0, 1};

    // First run should return 0 and initialize state
    rpm = convertValuesRPM(100, 1000, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(0, rpm);
    TEST_ASSERT_EQUAL_UINT(0, state.first_run);
    TEST_ASSERT_EQUAL_UINT32(100, state.last_count);
    TEST_ASSERT_EQUAL_UINT32(1000, state.last_time_ticks);

    // Second run with valid data (150 pulses in 600 ticks)
    // PPR=20, ticks_per_second=1000
    // rpm = 150 * 60 * 1000 / (20 * 600) = 750
    rpm = convertValuesRPM(250, 1600, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(750, rpm);

    // Third run with different data (80 pulses in 400 ticks)
    // rpm = 80 * 60 * 1000 / (20 * 400) = 600
    rpm = convertValuesRPM(330, 2000, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(600, rpm);

    // Fourth run with higher speed (200 pulses in 200 ticks)
    // rpm = 200 * 60 * 1000 / (20 * 200) = 3000
    rpm = convertValuesRPM(530, 2200, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(3000, rpm);

    // Fifth run with lower speed (30 pulses in 900 ticks)
    // rpm = 30 * 60 * 1000 / (20 * 900) = 100
    rpm = convertValuesRPM(560, 3100, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(100, rpm);
}

/* RSR-STM32-003 */
void test_RPM_invalid_arguments_UT_STM32_002(void) {

    UINT        rpm;
    t_rpm_state state = {0, 0, 1};

    rpm = convertValuesRPM(0, 0, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(0, rpm); 
    TEST_ASSERT_EQUAL_UINT(0, state.first_run);
}

/* RSR-STM32-004 */
void test_overflow_prevention_UT_STM32_003(void) {

    t_rpm_state state = {0, 0, 1};
    UINT        rpm;
    ULONG       period = 65535;

    // Initialize
    rpm = convertValuesRPM(65500, 1000, period, &state);
    TEST_ASSERT_EQUAL_UINT(0, rpm);

    // Counter wraps from 65500 to 100
    // pulses = period - 65500 + 100 + 1 = 136
    rpm = convertValuesRPM(100, 1500, period, &state);
    TEST_ASSERT_GREATER_THAN_UINT(999999999999990, rpm);
}

/* RSR-STM32-005 */
void test_rpm_value_bounds_UT_STM32_004(void) {

    UINT        rpm;
    t_rpm_state state = {0, 0, 1};
    
    // Initialize
    rpm = convertValuesRPM(0, 1000, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(0, rpm);
    
    // Huge pulse count in short time to exceed MAX_RPM
    rpm = convertValuesRPM(5000, 1001, 65535, &state);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(MAX_RPM, rpm);
}

/* RSR-STM32-006 */
void test_zero_time_delta_UT_STM32_005(void) {

    UINT        rpm;
    t_rpm_state state = {0, 0, 1};
    
    // Initialize
    rpm = convertValuesRPM(100, 1000, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(0, rpm);
    
    // Same time (delta_ticks == 0) should return 0
    rpm = convertValuesRPM(150, 1000, 65535, &state);
    TEST_ASSERT_EQUAL_UINT(0, rpm);
}
