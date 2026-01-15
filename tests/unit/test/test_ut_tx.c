#include "unity.h"
#include "utils.h"

// file being tested
TEST_SOURCE_FILE("rx_handler.c");

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void    test_rx_handler_valid_messages_UT_STM32_030(void) {
    t_rx_can_msg    msg;
    UINT            ret;

    memset(&msg, 0, sizeof(t_rx_can_msg));

    // Test emergency brake message
    msg.type = 0x100;
    ret = rx_handler(&msg);
    TEST_ASSERT_EQUAL_UINT(TX_SUCCESS, ret);

    // Test steering throttle message
    msg.type = 0x101;
    ret = rx_handler(&msg);
    TEST_ASSERT_EQUAL_UINT(TX_SUCCESS, ret);

    // Test unknown message
    msg.type = 0x102;
    ret = rx_handler(&msg);
    TEST_ASSERT_EQUAL_UINT(TX_QUEUE_ERROR, ret);
}
