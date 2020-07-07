# AP_FuelCell Library

Support for Intelligent Energy 650W/800W Fuel Cells. Original code written by Peter Hall and commissioned by ISS Aerospace. Ported and adapted to newer copter versions by IE (Yasin Mamodohanif). All credits to original author.

## Setup

1. Set FUELCEL_TYPE to 1 for Intelligent Energy 650W / 800W. 
   - Requires flight controller reboot once enabled.
2. Set serial port 
   - SerialX_Protocol to 25. Consult flight controller manual for port mappings
   - SerialX_Baud to 9. Baud rate 9600
3. Set Battery monitor
   - Set Batt_Monitor to 13 for tank pressure 
   - Set Batt2_Monitor to 14 for battery voltage
   - Disable failsafe voltages (BATT_CRT_VOLT = 0, BATT_LOW_VOLT = 0 and BATT_ARM_VOLT = 0) for any fuel cell mapped battery monitors
   - Set BATT_CAPACITY to 100 for any fuel cell mapped battery monitors. This maps tank pressure/battery percentage provided by fuel cell to battery capacity.
   - Set low (BATT_LOW_MAH) and critical (BATT_CRT_MAH) capacities and corresponding failsafes if required for any fuel cell mapped battery monitors
Note: Failsafes will only be triggered on battery type 13 (tank pressure). As the fuel cell provides no information about voltage, the battery monitor will report a fixed voltage of 1v. This requires the battery failsafe voltage to be disabled. The above batt_monitor mappings are for example purposes, they can be set to any battery monitor.

Failsafe set up: 
The are two parameters which are bit masks for what vehicle battery failsafe actions should be taken when the fuel cell reports it internal failsafes. FUELCEL_FS_LOW is a bitmask for internal fuel cell failsafes to the battery low failsafe action. FUELCEL_FS_CRIT is a bitmask to battery failsafe critical actions. Note that the apropriate failsafe action must be set with the BATT_FS_LOW_ACT and BATT_FS_CRT_ACT parameters. Below is a table of values to set the FUELCEL_FS_LOW and FUELCEL_FS_CRIT and the corresponding failsafe description. As the parameters are bitmask multiple values can be added.

| Hex   | Description |
|-------|-------------|
|0x80000000| Stack OT #1|
|0x40000000| Stack OT #2|
|0x20000000| Battery UV|
|0x10000000| Battery OT|
|0x08000000| No Fan|
|0x04000000| Fan Overrun|
|0x02000000| Stack OT #1|
|0x01000000| Stack OT #2|
|0x00800000| Battery UV|
|0x00400000| Battery OT|
|0x00200000| Master Start Timeout|
|0x00100000| Master Stop Timeout|
|0x00080000| Start Under Pressure|
|0x00040000| Tank Under Pressure|
|0x00020000| Tank Low Pressure|
|0x00010000| Safety Flag Before Master EN|
|0x00008000| Deny Start - Stack 1 Under-Temperature|
|0x00004000| Deny Start - Stack 2 Under-Temperature|
|0x00002000| Stack 1 Under-Temperature|
|0x00001000| Stack 1 Under-Temperature|
|0x00000800| Battery UV Warning|
|0x00000400| Battery UV Deny Master Enable|
|0x00000200| Fan Pulse Load Abort|
|0x00000100| Stack Under-Voltage|
|0x00000080| System Over-Loaded|
|0x00000040| Over Voltage Over Current|
|0x00000020| Invalid Serial Number|
|0x00000010| Battery Charger Fault|
|0x00000008| Battery UT|



##  Usage

The battery monitor will fail arming checks if:
- The driver is not healthy, it has not received any valid communication from the fuel cell in the last 5 seconds.
- The fuel cell is in any other state except running
- There are any fuel cell failsafes

The fuel cell will report any change in state and any failsafes that happen. It will not report failsafes when they clear. The fuel cell will take no action on change of state.
