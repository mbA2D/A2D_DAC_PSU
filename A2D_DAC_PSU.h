/* A2D Electronics Relay Board Library
 * Written By: Micah Black
 * Date: Dec 9, 2022
 *
 * Controls a DAC connected to a TI bidirectional DC-DC converter EVM (PMP15043).
 * The DAC controls VSET and ISET to set the voltage and current for battery testing.
 * 
 */


#ifndef A2D_Power_Board_h
#define A2D_Power_Board_h

#include "Arduino.h"
#include <A2D_DAC_PSU_V1.0.h> //header file with pins, etc
#include <Wire.h>
#include <DACX0502.h>
#include <EEPROM.h>

class A2D_DAC_PSU
{
	public:
		A2D_DAC_PSU(); //constructor
		
		//Configuration
		void init();
		void reset();
		
		//Interface
		void set_current(float value);
		void set_voltage(float value);
		
		void enable_psu();
		void disable_psu();
		void enable_eload();
		void disable_eload();
		
		void set_led(bool state);
		void set_dac_i2c_addr(uint8_t addr);
		
		void reset_calibration();
		void save_calibration();

	private:
		//************METHODS****************
		void reset_current_calibration();
		void reset_voltage_calibration();
		void _init_cal_from_eeprom();
		
		//*********VARIABLES/CLASSES*********

		//scaling for voltages
		float _v_scaling;
		float _i_scaling;
		float _v_offset;
		float _i_offset;
		
		uint32_t _serial;
		uint8_t _ee_initialized;
		
		int _ee_addr_initialized;
		int _ee_addr_serial;
		int _ee_addr_v_off;
		int _ee_addr_i_off;
		int _ee_addr_v_scale;
		int _ee_addr_i_scale;
		
		DACX0502 *_dac;
		uint8_t _dac_addr;
};

#endif