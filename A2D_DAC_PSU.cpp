/* A2D Electronics DAC Board Library
 * Written By: Micah Black
 * Date: Aug 6, 2023
 *
 *
 */

#include <A2D_DAC_PSU.h>
#include <DACX0502.h>

A2D_DAC_PSU::A2D_DAC_PSU()
{	
	if (TWCR == 0) 
	{
		Wire.begin();
	}
	Wire.setWireTimeout(3000, true);
	
	
	pinMode(A2D_DAC_PSU_PSU_EN_PIN, OUTPUT);
	disable_psu();
	pinMode(A2D_DAC_PSU_ELOAD_EN_PIN, OUTPUT);
	disable_eload();
	
	_dac = new DACX0502();
	_dac_addr = A2D_DAC_PSU_MIN_I2C_ADDR;
	
	_i_scaling = A2D_DAC_PSU_I_DEFAULT_SCALING;
	_v_scaling = A2D_DAC_PSU_V_DEFAULT_SCALING;
	_v_offset = A2D_DAC_PSU_I_DEFAULT_OFFSET;
	_i_offset = A2D_DAC_PSU_V_DEFAULT_OFFSET;
	
	_ee_addr_initialized = 50; //1
	_ee_addr_serial = _ee_addr_initialized + sizeof(uint8_t); //4
	_ee_addr_v_off = _ee_addr_serial + sizeof(uint32_t); //4
	_ee_addr_i_off = _ee_addr_v_off + sizeof(float); //4
	_ee_addr_v_scale = _ee_addr_i_off + sizeof(float); //4
	_ee_addr_i_scale = _ee_addr_v_scale + sizeof(float); //4  //21 bytes total
}

void A2D_DAC_PSU::init()
{
	pinMode(A2D_DAC_PSU_LED_PIN, OUTPUT);
	_dac->init(_dac_addr);
	
	//configure DAC
	_dac->shut_down_ref(false); //use internal reference
	_dac->set_buf_gain_a(DACX0502_BUFGAIN_1);//buf gain of 1.0 for current (max 5A)
	_dac->set_buf_gain_b(DACX0502_BUFGAIN_2);//buf gain of 2.0 for voltage (max 5V)
	
	//calibrate DAC - TODO
	
}

void A2D_DAC_PSU::enable_eload()
{
	disable_psu();
	digitalWrite(A2D_DAC_PSU_ELOAD_EN_PIN, A2D_DAC_PSU_ELOAD_EN);
}

void A2D_DAC_PSU::disable_eload()
{
	digitalWrite(A2D_DAC_PSU_ELOAD_EN_PIN, !A2D_DAC_PSU_ELOAD_EN);
}

void A2D_DAC_PSU::enable_psu()
{
	disable_eload();
	digitalWrite(A2D_DAC_PSU_PSU_EN_PIN, A2D_DAC_PSU_PSU_EN);
}

void A2D_DAC_PSU::disable_psu()
{
	digitalWrite(A2D_DAC_PSU_PSU_EN_PIN, !A2D_DAC_PSU_PSU_EN);
}


void A2D_DAC_PSU::reset()
{
	disable_eload();
	disable_psu();
	
	//LED
	digitalWrite(A2D_DAC_PSU_LED_PIN, LOW);
	
	_dac->reset();
	init();
}

void A2D_DAC_PSU::set_current(float value)
{
	_dac->set_dac_a((value - _i_offset)/_i_scaling);
}

void A2D_DAC_PSU::set_voltage(float value)
{
	_dac->set_dac_b((value - _v_offset)/_v_scaling);
}


/*
void A2D_DAC_PSU::calibrate_current(float p1_meas, float p1_act, float p2_meas, float p2_act)
{
	//calculate new offset (b) and scaling (m) in:  actual = m * measured + b
	_i_scaling = (p2_act - p1_act) / (p2_meas - p1_meas); //rise in actual / run in measured
	_i_offset = p2_act - _i_scaling * p2_meas; //b = actual - m * measured
	
	//save to eeprom
	EEPROM.put(_ee_addr_i_off, _i_offset);
	EEPROM.put(_ee_addr_i_scale, _i_scaling);
}

void A2D_DAC_PSU::calibrate_voltage(float p1_meas, float p1_act, float p2_meas, float p2_act)
{
	//calculate new offset (b) and scaling (m) in:  actual = m * measured + b
	_v_scaling = (p2_act - p1_act) / (p2_meas - p1_meas); //rise in actual / run in measured
	_v_offset = p2_act - _i_scaling * p2_meas; //b = actual - m * measured
	
	//save to eeprom
	EEPROM.put(_ee_addr_v_off, _v_offset);
	EEPROM.put(_ee_addr_v_scale, _v_scaling);
}

void A2D_DAC_PSU::_init_cal_from_eeprom()
{
	//update all calibration values from EEPROM
	EEPROM.get(_ee_addr_v_off, _v_offset);
	EEPROM.get(_ee_addr_i_off, _i_offset);
	EEPROM.get(_ee_addr_v_scale, _v_scaling);
	EEPROM.get(_ee_addr_i_scale, _i_scaling);
}

//resets to the default calibration - assumes all components have 0% tolerance.
void A2D_DAC_PSU::reset_calibration()
{
	reset_current_calibration();
	reset_voltage_calibration();
}

void A2D_DAC_PSU::reset_voltage_calibration()
{
	_v_offset = 0;
	_v_scaling = A2D_DAC_PSU_V_SCALING;
	
	EEPROM.put(_ee_addr_v_off, _v_offset);
	EEPROM.put(_ee_addr_v_scale, _v_scaling);
}

void A2D_DAC_PSU::reset_current_calibration()
{
	_i_offset = 0;
	_i_scaling = A2D_DAC_PSU_I_SCALING;
	
	EEPROM.put(_ee_addr_i_off, _i_offset);
	EEPROM.put(_ee_addr_i_scale, _i_scaling);
}

*/

void A2D_DAC_PSU::reset_calibration()
{
	_i_scaling = A2D_DAC_PSU_I_DEFAULT_SCALING;
	_v_scaling = A2D_DAC_PSU_V_DEFAULT_SCALING;
	_v_offset = A2D_DAC_PSU_I_DEFAULT_OFFSET;
	_i_offset = A2D_DAC_PSU_V_DEFAULT_OFFSET;
}


void A2D_DAC_PSU::set_led(bool state)
{
	digitalWrite(A2D_DAC_PSU_LED_PIN, state);
}

void A2D_DAC_PSU::set_dac_i2c_addr(uint8_t addr)
{
	if (addr >= A2D_DAC_PSU_MIN_I2C_ADDR && addr <= A2D_DAC_PSU_MAX_I2C_ADDR)
	{
		_dac_addr = addr;
	}
	reset();
}
