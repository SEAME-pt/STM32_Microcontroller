ID: SR-STM32-001
Title: Speed Sensor
Type: Functional
Priority: High
Source: System

Description:
The system shall correctly process wheel speed telemetry from speed sensor.

Rationale:
Wheel speed telemetry is required for monitoring vehicle behavior and system status with the intentio of display this data into a display.

Verification:
System Test

Acceptance Criteria:
Each of this test should pass:
 - SWR-STM32-002
 - RSR-STM32-003
 - RSR-STM32-004
 - RSR-STM32-005
 - RSR-STM32-006
 - RSR-STM32-007
 - RSR-STM32-008
 - RSR-STM32-009
 - RSR-STM32-010

 ---

ID: SWR-STM32-002
Title: Right calculation of RPM
Type: Functional
Priority: High
Source: System

Description:
The software shall calculate wheel speed in RPM based on the number of sensor pulses 
measured during a fixed sampling period.

Rationale:
Matematical formula to calculate RPM must be verified to proceed to every other test that shares sensor speed.

Parent Requirement:
SR-STM32-001

Related Components:
- Sensor Thread

Verification:
Unit Test (UT-STM32-001)

Acceptance Criteria:
- RPM equals (pulses / pulses_per_rev) × (60 / sampling_time).

---

ID: RSR-STM32-003
Title: RPM invalid arguments
Type: Safety
Priority: Medium
Source: System

Description:
With invalid argumets, the system must proced with normal procedure.

Rationale:
Important to understand what would happen if timer values are absurd due to a system error, and how the calculation function will deal with it.

Parent Requirement:
SR-STM32-001

Related Components:
- Sensor Thread
- Timer
- Sensor Speed counter

Verification:
Unit Test (RSR-STM32-003)

Acceptance Criteria:
- Return must be equal to 0

---

ID: RSR-STM32-004
Title: Overflow prevention
Type: Safety
Priority: Medium
Source: System

Description:
Shall detect the RPM isn t right, so it must return MAX_RPM (1000)

Rationale:
If, somehow the RPM is greater than physically the car goes, to protect overflow errors, the return will be clamped to 1000.

Parent Requirement:
SR-STM32-001

Related Components:
- Sensor Thread

Verification:
Unit Test (RSR-STM32-004)

Acceptance Criteria:
- Return must be equal to 1000

---

ID: RSR-STM32-004
Title: RPM value bounds
Type: Safety
Priority: Low
Source: Stakeholder

Description:
Shall prevent some undifined error

Rationale:
Sensor speed component can odly detect huge pulse count in a short time,
if so, probably something went wrong and the program must detect it.

Parent Requirement:
SR-STM32-001

Related Components:
- Sensor Thread
- Speed sensor

Verification:
Unit Test (RSR-STM32-005)

Acceptance Criteria:
- Return must be equal to 1000
