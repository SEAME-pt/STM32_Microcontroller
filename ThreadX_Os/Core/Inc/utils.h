#ifndef UTILS_H_
 #define UTILS_H_

#include <stdint.h>

// Use ThreadX types if available, otherwise define our own
#ifndef TX_API_H
    typedef uint32_t    UINT;
    typedef uint32_t    ULONG;
    typedef void        VOID;
    #define TX_SUCCESS      0
    #define TX_QUEUE_ERROR  1
#endif

// Timer ticks per second definition
#define TX_TIMER_TICKS_PER_SECOND    1000

// CAN message types
typedef enum {
  CAN_MSG_SPEED,
  CAN_MSG_STEERING_THROTTLE,
  CAN_MSG_BATTERY
} e_can_msg_type;

// TX CAN message structure
typedef struct s_tx_can_message {
  e_can_msg_type type;
  uint8_t        data[8];
} t_tx_can_msg;

// RX CAN message structure
typedef struct s_rx_can_message {
  uint32_t       type;
  uint8_t        data[8];
} t_rx_can_msg;

//Maximum RPM value to prevent overflow
#define MAX_RPM         5000

//Pulses Per Revolution
#define PPR            20

// RPM calculation state, created for hardware abstraction and testability
typedef struct s_rpm_state {
    ULONG   last_time_ticks;
    ULONG   last_count;
    UINT    first_run;
} t_rpm_state;

UINT    convertValuesRPM(
    ULONG count, 
    ULONG ticks, 
    ULONG period,
    t_rpm_state *state);

UINT    rx_handler(const t_rx_can_msg *msg);

#endif