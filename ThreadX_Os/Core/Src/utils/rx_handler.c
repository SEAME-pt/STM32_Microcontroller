#include "utils.h"

UINT    rx_handler(const t_rx_can_msg *msg)
{
    UINT ret;

    ret = TX_SUCCESS;
    switch (msg->type)
    {
        case 0x100:  // emergency break
            break;

        case 0x101: // steering throttle
            break;

        default:
            ret = TX_QUEUE_ERROR;
            break;
    }
    return (ret);
}
