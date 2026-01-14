#include "unity.h" 
#include "speed_rpm.h" 

void setUp(void) { 
    // set stuff up here 
} 

void tearDown(void) { 
    // clean stuff up here 
} 

/* SWR-STM32-002 */ 
void test_right_calculation_of_RPM_UT_STM32_001(void) {

    t_rpm_state state = {0, 0, 1}; 
    UINT rpm; 

    // First run should return 0 and initialize state 
    rpm = convertValuesRPM(100, 1000, 65535, 1000, &state); 
    TEST_ASSERT_EQUAL_UINT(0, rpm); 
    TEST_ASSERT_EQUAL_UINT(0, state.first_run); 
    TEST_ASSERT_EQUAL_UINT32(100, state.last_count); 
    TEST_ASSERT_EQUAL_UINT32(1000, state.last_time_ticks);

    // Second run with valid data (100 pulses in 500 ticks) 
    // Assuming PPR=20, TX_TIMER_TICKS_PER_SECOND=1000 
    // rpm = 100 * 60 * 1000 / (20 * 500) = 600 
    rpm = convertValuesRPM(200, 1500, 65535, 1000, &state); 
    TEST_ASSERT_GREATER_THAN_UINT(0, rpm); 
} 

/* RSR-STM32-003 */ 
void test_RPM_invalid_arguments_UT_STM32_002(void) {

    t_rpm_state state = {0, 0, 1}; 
    UINT rpm; 

    rpm = convertValuesRPM(0, 0, 65535, 1000, &state); 
    TEST_ASSERT_EQUAL_UINT(0, rpm); TEST_ASSERT_EQUAL_UINT(0, state.first_run); 
} 

/* RSR-STM32-004 */ 
void test_overflow_prevention_UT_STM32_003(void) { 
    
    t_rpm_state state = {0, 0, 1}; 
    UINT rpm; 
    ULONG period = 65535; 
    
    // Initialize 
    rpm = convertValuesRPM(65500, 1000, period, 1000, &state); 
    TEST_ASSERT_EQUAL_UINT(0, rpm); 
    
    // Counter wraps from 65500 to 100 
    // pulses = period - 65500 + 100 + 1 = 136 
    rpm = convertValuesRPM(100, 1500, period, 1000, &state); 
    TEST_ASSERT_GREATER_THAN_UINT(0, rpm); 
} 

/* RSR-STM32-005 */ 
void test_rpm_value_bounds_UT_STM32_004(void) { 
    
    t_rpm_state state = {0, 0, 1}; 
    UINT rpm; 
    
    // Initialize 
    rpm = convertValuesRPM(0, 1000, 65535, 1000, &state); 
    TEST_ASSERT_EQUAL_UINT(0, rpm); 
    
    // Huge pulse count in short time to exceed MAX_RPM 
    rpm = convertValuesRPM(10000, 1001, 65535, 1000, &state); 
    TEST_ASSERT_LESS_OR_EQUAL_UINT(MAX_RPM, rpm); 
} 

int main(void) { 
    UNITY_BEGIN(); 
    RUN_TEST(test_right_calculation_of_RPM_UT_STM32_001); 
    RUN_TEST(test_RPM_invalid_arguments_UT_STM32_002); 
    RUN_TEST(test_overflow_prevention_UT_STM32_003); 
    RUN_TEST(test_rpm_value_bounds_UT_STM32_004); 
    return UNITY_END(); 
}