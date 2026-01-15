#include "utils.h"

UINT    tx_handler(const t_rx_can_msg *msg)
{
    UINT ret;

    ret = TX_SUCCESS;
    switch (msg->type)
    {
        case 0x100:  // emergency break
            uart_send("Emergency break received!\r\n");
            break;

        case 0x101: // steering throttle
            uart_send("Steering and throttle message received!\r\n");
            break;

        case 0x102: // mode
            uart_send("Mode change message received!\r\n");
            break;
        default:
            ret = TX_QUEUE_ERROR;
            break;
    }
    return (ret);
}
