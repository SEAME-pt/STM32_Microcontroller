### System Requirement SR-STM32-001

| Requirement ID | Verification | Code Location    | Test ID      | Title
| -------------- | ------------ | ---------------- | ------------ | ------------
| SWR-STM32-002  | Pass         | speed_rpm.c      | UT-STM32-001 | Right calculation of RPM
| RSR-STM32-003  | Pass         | speed_rpm.c      | UT_STM32_002 | RPM invalid arguments
| RSR-STM32-004  | Pass         | speed_rpm.c      | UT_STM32_003 | Overflow prevention
| RSR-STM32-005  | Pass         | speed_rpm.c      | UT_STM32_004 | RPM value bounds
| RSR-STM32-006  | Fail         | speedSensor.c    | IT_STM32_001 | Hardware invalid arguments
| RSR-STM32-007  | Fail         | speedSensor.c    | IT_STM32_002 | Broken timer
| RSR-STM32-008  | Fail         | speedSensor.c    | IT_STM32_003 | Timer init validation
| RSR-STM32-009  | Fail         | speedSensor.c    | IT_STM32_004 | RPM Debug Output

| Line Coverage | Function Coverage |
| ------------- | ----------------- |
| 94.12%        | 100%              |
