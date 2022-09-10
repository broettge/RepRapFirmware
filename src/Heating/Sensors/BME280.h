/*
 * BME280.h
 *
 *  Created on: 10 Sept 2022
 *      Author: David
 */

#ifndef SRC_HEATING_SENSORS_BME280_H_
#define SRC_HEATING_SENSORS_BME280_H_

#include "SpiTemperatureSensor.h"

#if SUPPORT_BME280

#include "AdditionalOutputSensor.h"
#include "bme280_defs.h"

class BME280TemperatureSensor : public SpiTemperatureSensor
{
public:
	BME280TemperatureSensor(unsigned int sensorNum) noexcept;

	GCodeResult Configure(GCodeBuffer& gb, const StringRef& reply, bool& changed) override THROWS(GCodeException);

#if SUPPORT_REMOTE_COMMANDS
	GCodeResult Configure(const CanMessageGenericParser& parser, const StringRef& reply) noexcept override; // configure the sensor from M308 parameters
#endif

	const uint8_t GetNumAdditionalOutputs() const noexcept override { return 2; }
	TemperatureError GetLatestTemperature(float& t, uint8_t outputNumber = 0) noexcept override;
	void Poll() noexcept override;
	const char *GetShortSensorType() const noexcept override { return TypeName; }

	static constexpr const char *TypeName = "bme280temperature";

private:
	TemperatureError bme280_init() noexcept;
	TemperatureError bme280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len) noexcept;
	TemperatureError bme280_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len) noexcept;
	TemperatureError get_calib_data() noexcept;
	void bme280_compensate_data(const bme280_uncomp_data *uncomp_data) noexcept;
	float compensate_temperature(const struct bme280_uncomp_data *uncomp_data) noexcept;
	float compensate_pressure(const struct bme280_uncomp_data *uncomp_data) const noexcept;
	float compensate_humidity(const struct bme280_uncomp_data *uncomp_data) const noexcept;
	void parse_temp_press_calib_data(const uint8_t *reg_data) noexcept;
	void parse_humidity_calib_data(const uint8_t *reg_data) noexcept;
	TemperatureError bme280_soft_reset() noexcept;
	TemperatureError bme280_get_sensor_data() noexcept;
	void bme280_parse_sensor_data(const uint8_t *reg_data, struct bme280_uncomp_data *uncomp_data) noexcept;

	bme280_dev dev;
	uint32_t lastReadTime;
    float compPressure;			    /*< Compensated pressure */
    float compTemperature;    		/*< Compensated temperature */
    float compHumidity;    			/*< Compensated humidity */
};

// This class represents a DHT humidity sensor
class BME280PressureSensor : public AdditionalOutputSensor
{
public:
	BME280PressureSensor(unsigned int sensorNum) noexcept;
	~BME280PressureSensor() noexcept;

	const char *GetShortSensorType() const noexcept override { return TypeName; }

	static constexpr const char *TypeName = "bme280pressure";
};

// This class represents a DHT humidity sensor
class BME280HumiditySensor : public AdditionalOutputSensor
{
public:
	BME280HumiditySensor(unsigned int sensorNum) noexcept;
	~BME280HumiditySensor() noexcept;

	const char *GetShortSensorType() const noexcept override { return TypeName; }

	static constexpr const char *TypeName = "bme280humidity";
};

#endif	// SUPPORT_BME280

#endif /* SRC_HEATING_SENSORS_BME280_H_ */
