/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <AP_Param/AP_Param.h>
#include <AP_SerialManager/AP_SerialManager.h>
#include <AP_BattMonitor/AP_BattMonitor.h>
#include <GCS_MAVLink/GCS.h>

#define TERM_BUFFER 12          // max length of term we expect
#define HEALTHY_TIMEOUT_MS 5000 // time for driver to be marked un-healthy

// failsafe messages
#define TEMP_1_CRIT         "Fuel Cell: Stack temperature 1 above critical (58C) limit"
#define TEMP_2_CRIT         "Fuel Cell: Stack temperature 2 above critical (58C) limit"
#define BAT_VOLT_CRIT       "Fuel Cell: Battery voltage below critical (19V) limit"
#define BAT_TEMP_CRIT       "Fuel Cell: Battery temperature above critical (65C) limit"
#define FAN_ERROR           "Fuel Cell: Fan running and fan current below limit"
#define FAN_ERROR2          "Fuel Cell: Fan stopped and fan current above limit"
#define TEMP_1_SHUT         "Fuel Cell: Stack temperature 1 above shutdown (57C) limit"
#define TEMP_2_SHUT         "Fuel Cell: Stack temperature 2 above shutdown (57C) limit"
#define BAT_VOLT_SHUT       "Fuel Cell: Battery voltage below shutdown (19.6V) limit"
#define BAT_TEMP_SHUT       "Fuel Cell: Battery temperature above shutdown (60C) limit"
#define START_TIMEOUT       "Fuel Cell: Master State in Starting for unexpected length of time"
#define STOP_TIMEOUT        "Fuel Cell: Master State in Stopping for unexpected length of time"
#define START_UNDER_PRESS   "Fuel Cell: Tank under pressure (<6barg) for system start"
#define TANK_UNDER_PRESS    "Fuel Cell: Tank pressure below shutdown (5barg) limit"
#define TANK_LOW_PRESS      "Fuel Cell: Tank pressure low (<15barg)"
#define SAFETY_FLAG         "Fuel Cell: Safety Flag, unexpected low safety line"
#define STACK_START_UT_1    "Fuel Cell: Deny start - Stack temperature 1 below (5C) limit"
#define STACK_START_UT_2    "Fuel Cell: Deny start - Stack temperature 2 below (5C) limit"
#define STACK_UT_1          "Fuel Cell: Stack temperature 1 below (5C) limit"
#define STACK_UT_2          "Fuel Cell: Stack temperature 2 below (5C) limit"
#define BAT_UV              "Fuel Cell: Battery voltage low (<21.6V)"
#define BAT_UV_START        "Fuel Cell: Deny start - Battery voltage low (<21.6V)"
#define FAN_PULSE_ABORT     "Fuel Cell: Fan Pulse aborted"
#define STACK_UV            "Fuel Cell: Stack voltage below threshold"
#define SYS_OVERLOAD        "Fuel Cell: Stack voltage below threshold, battery at max ouput"
#define OVOC                "Fuel Cell: Over voltage/over current safety hardware trip"
#define INVALID_SERIAL      "Fuel Cell: Invalid serial number"
#define BAT_CHARGER_FAULT   "Fuel Cell: Battery charger fault"
#define BAT_UT              "Fuel Cell: Battery thermistor damaged or not connected"

// statues messages
#define STATUS_START    "Fuel Cell: Status Starting"
#define STATUS_READY    "Fuel Cell: Status Ready"
#define STATUS_RUN      "Fuel Cell: Status Running"
#define STATUS_FAULT    "Fuel Cell: Status Fault"
#define STATUS_BATTERY  "Fuel Cell: Status Battery Only"


class AP_FuelCell
{

public:
    AP_FuelCell();

    /* Do not allow copies */
    AP_FuelCell(const AP_FuelCell &other) = delete;
    AP_FuelCell &operator=(const AP_FuelCell&) = delete;

    static AP_FuelCell *get_singleton();

    // Return true if datalogger is enabled
    bool enabled() const;

    // Initialize the datalogger object and prepare it for use
    void init();

    // Update datalogging
    void update();

    // get the tank remaining ratio
    float get_tank() const;

    // get the battery remaining ratio
    float get_battery() const;

    // check if readings are health
    bool healthy() const;

    // check for arming
    bool arming_checks(char * buffer, size_t buflen) const;

    // check for failsafes
    AP_BattMonitor::BatteryFailsafe update_failsafes() const;

    // parameter block
    static const struct AP_Param::GroupInfo var_info[];

private:

    // Parameters
    AP_Int8 _type;    // type of fuel cell
    AP_Int32 _low_fs_bitmask;
    AP_Int32 _crit_fs_bitmask;

    // Pointer to serial uart
    AP_HAL::UARTDriver *_uart = nullptr; 

    enum FUEL_CELL_TYPE {
        _FUEL_CELL_NONE = 0,
        _FUEL_CELL_IE   = 1,
    };

    enum FUEL_CELL_FAILSAFE {
        _FUEL_CELL_FAILSAFE_STACK_OT1               = (1 << 31),
        _FUEL_CELL_FAILSAFE_STACK_OT2               = (1 << 30),
        _FUEL_CELL_FAILSAFE_BAT_UV                  = (1 << 29),
        _FUEL_CELL_FAILSAFE_BAT_OT                  = (1 << 28),
        _FUEL_CELL_FAILSAFE_NO_FAN                  = (1 << 27),
        _FUEL_CELL_FAILSAFE_FAN_OVERRUN             = (1 << 26),
        _FUEL_CELL_FAILSAFE_STACK_OT1_2             = (1 << 25),
        _FUEL_CELL_FAILSAFE_STACK_OT2_2             = (1 << 24),
        _FUEL_CELL_FAILSAFE_BAT_UV_2                = (1 << 23),
        _FUEL_CELL_FAILSAFE_BAT_OT_2                = (1 << 22),
        _FUEL_CELL_FAILSAFE_START_TIMEOUT           = (1 << 21),
        _FUEL_CELL_FAILSAFE_STOP_TIMEOUT            = (1 << 20),
        _FUEL_CELL_FAILSAFE_START_UNDER_PRESSURE    = (1 << 19),
        _FUEL_CELL_FAILSAFE_TANK_UNDER_PRESSURE     = (1 << 18),
        _FUEL_CELL_FAILSAFE_TANK_LOW_PRESSURE       = (1 << 17),
        _FUEL_CELL_FAILSAFE_SAFETY_FLAG             = (1 << 16),
        _FUEL_CELL_FAILSAFE_STACK_START_UT_1        = (1 << 15),
        _FUEL_CELL_FAILSAFE_STACK_START_UT_2        = (1 << 14),
        _FUEL_CELL_FAILSAFE_STACK_UT_1              = (1 << 13),
        _FUEL_CELL_FAILSAFE_STACK_UT_2              = (1 << 12),
        _FUEL_CELL_FAILSAFE_BAT_UV                  = (1 << 11),
        _FUEL_CELL_FAILSAFE_BAT_UV_START            = (1 << 10),
        _FUEL_CELL_FAILSAFE_FAN_PULSE_ABORT         = (1 << 9),
        _FUEL_CELL_FAILSAFE_STACK_UV                = (1 << 8),
        _FUEL_CELL_FAILSAFE_SYS_OVERLOAD            = (1 << 7),
        _FUEL_CELL_FAILSAFE_OVOC                    = (1 << 6),
        _FUEL_CELL_FAILSAFE_INVALID_SERIAL          = (1 << 5),
        _FUEL_CELL_FAILSAFE_BAT_CHARGER_FAULT       = (1 << 4),
        _FUEL_CELL_FAILSAFE_BAT_UT                  = (1 << 3),


    };

    enum  FUEL_CELL_STATE {
        _FUEL_CELL_STATE_STARTING       = 0,
        _FUEL_CELL_STATE_READY          = 1,
        _FUEL_CELL_STATE_RUNNING        = 2,
        _FUEL_CELL_STATE_FAULT          = 3,
        _FUEL_CELL_STATE_BATTERY_ONLY   = 8,
    };

    // The fuel cell state structure
    struct BattMonitor_State {
        float       tank_pct;              // tank percentage remaining
        float       battery_pct;           // battery percentage remaining
        uint32_t    failsafe;              // the failsafe state
        uint32_t    last_failsafe;         // the previous failsafe state
        uint8_t     state;                 // the PSU state
        uint8_t     last_state;            // the previous PSU state
        uint32_t    last_time_ms;          // time we got a reading
        bool        healthy;               // is the driver working
    };
    BattMonitor_State _state;

    // tempary state params
    float _temp_tank_pct;
    float _temp_battery_pct;
    uint8_t _temp_state;
    uint32_t _temp_failsafe;

    // decodeing vars
    char _term[TERM_BUFFER];    // term buffer
    bool _sentence_valid;       // is current sentence valid so far
    uint8_t _term_number;       // term index within the current sentence
    uint8_t _term_offset;       // offset within the _term buffer where the next character should be placed
    bool _in_string;            // true if we should be decoding

    // update IE backend
    void update_IE();

    // add a single character to the buffer and attempt to decode
    // returns true if a complete sentence was successfully decoded or if the buffer is full
    bool decode(char c);

    void decode_latest_term();

    // check if we should notify on any change of fuel cell state
    void check_status();

    static AP_FuelCell *_singleton;
};

namespace AP {
    AP_FuelCell *fuelcell();
};
