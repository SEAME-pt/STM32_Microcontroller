#include "utils.h"

// RPM = pulse / PPR * (60 / dt_seconds)
// RPM calculation from timer values
UINT convertValuesRPM(
    ULONG count,
    ULONG ticks,
    ULONG period,
    t_rpm_state *state)
{
    if (state->first_run)
    {
        state->last_count = count;
        state->last_time_ticks = ticks;
        state->first_run = 0;
        return (0);
    }

    // Calculate time in ticks
    ULONG delta_ticks = ticks - state->last_time_ticks;
    if (delta_ticks == 0)
        return (0);

    // Calculate pulses (handles counter overflow)
    ULONG pulses = (count >= state->last_count)
                    ? (count - state->last_count)
                    : (period - state->last_count + count + 1);

    // Update vars for next call
    state->last_count = count;
    state->last_time_ticks = ticks;

    // Converts to RPM
    UINT rpm = (UINT)(pulses * 60 * TX_TIMER_TICKS_PER_SECOND / (PPR * delta_ticks));

    if (rpm > MAX_RPM) rpm = MAX_RPM;
    return (rpm);
}
