#ifndef SPEED_RPM_H_
#define SPEED_RPM_H_

#include <stdint.h>

// Use ThreadX types if available, otherwise define our own
#ifndef TX_API_H
typedef uint32_t    UINT;
typedef uint32_t    ULONG;
typedef void        VOID;
#endif

//Maximum RPM value to prevent overflow
#define MAX_RPM         10000

//Pulses Per Revolution
#define PPR            20.0

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
    ULONG ticks_per_second,
    t_rpm_state *state);

#endif