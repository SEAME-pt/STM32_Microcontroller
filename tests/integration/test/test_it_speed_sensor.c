#include "unity.h"
#include "app_threadx.h"
#include "hal_stubs.h"

// Tell Ceedling to compile this source file for the test
TEST_SOURCE_FILE("speedSensor.c")

// Production function to test
extern VOID rpm_debug_print(ULONG rpm, ULONG cr1_reg, ULONG cnt_reg);

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

/* RSR-STM32-006 */
void test_hardware_invalid_arguments_IT_STM32_001(void) {

}

/* RSR-STM32-007 */
void test_broken_timer_IT_STM32_002(void) {

}

/* RSR-STM32-008 */
void test_init_validation_IT_STM32_003(void) {

}

/* RSR-STM32-009 */
void test_rpm_debug_output_IT_STM32_004(void) {
   
}