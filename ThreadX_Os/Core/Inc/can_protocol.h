#ifndef CAN_PROTOCOL_H
 #define CAN_PROTOCOL_H

#include "app_threadx.h"

// CAN message types
typedef enum {
  CAN_MSG_SPEED,
  CAN_MSG_BATTERY,
  CAN_MSG_HEARTBEAT,
} e_can_msg_type;

// TX CAN message structure (sending)
typedef struct s_tx_can_message {
  e_can_msg_type    type;
  uint8_t           data[8];
} t_tx_can_msg;

// RX CAN message structure (receiving)
typedef struct s_rx_can_message {
  uint16_t   type;
  uint8_t   data[8];
  uint8_t   len;
} t_rx_can_msg;

// Steering/throttle CAN message instructions
typedef struct s_i2c_message {
    int8_t steering;
    int8_t throttle;
} t_i2c_msg;

//static const uint8_t  dlc_to_len[16] = {0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};

HAL_StatusTypeDef   rx_receive(t_rx_can_msg *msg);

#endif