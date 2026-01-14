#ifndef SPEED_RPM_H_
#define SPEED_RPM_H_

#include <stdint.h>

// Use ThreadX types if available, otherwise define our own
#ifndef TX_API_H
    typedef uint32_t    UINT;
    typedef uint32_t    ULONG;
    typedef void        VOID;
#endif

#ifndef TX_TIMER_TICKS_PER_SECOND
    #define TX_TIMER_TICKS_PER_SECOND    1000
#endif

//Maximum RPM value to prevent overflow
#define MAX_RPM         10000

//Pulses Per Revolution
#define PPR            20

// RPM calculation state, created for hardware abstraction and testability
typedef struct s_rpm_state {
    ULONG   last_time_ticks;
    ULONG   last_count;
    UINT    first_run;
} t_rpm_state;

UINT  convertValuesRPM(
    ULONG count, 
    ULONG ticks, 
    ULONG period,
    t_rpm_state *state);

#endif