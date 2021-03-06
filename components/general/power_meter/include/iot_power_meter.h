/*
  * ESPRESSIF MIT License
  *
  * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
  *
  * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
  * it is free of charge, to any person obtaining a copy of this software and associated
  * documentation files (the "Software"), to deal in the Software without restriction, including
  * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
  * to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all copies or
  * substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  */

#ifndef _IOT_POWER_METER_H_
#define _IOT_POWER_METER_H_

#include "sdkconfig.h"
#if CONFIG_POWER_METER_ENABLE

#include "esp_system.h"
#include "driver/pcnt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PM_BOTH_VC = 0,                 /**< voltage and current are measured at the same time by different pin */
    PM_SINGLE_CURRENT,              /**< only current is measured now */
    PM_SINGLE_VOLTAGE,              /**< only voltage is measured now */
} pm_mode_t;

typedef enum {
    PM_POWER = 0,
    PM_VOLTAGE,
    PM_CURRENT,
} pm_value_type_t;

typedef struct {
    uint8_t power_io_num;               /**< gpio number of the pin outputing power */
    pcnt_unit_t power_pcnt_unit;        /**< pulse count unit of power pin */
    uint32_t power_ref_param;           /**< reference value to calculate actual power value */
    uint8_t voltage_io_num;             /**< gpio number of the pin outputing voltage */
    pcnt_unit_t voltage_pcnt_unit;      /**< pulse count unit of voltage pin */
    uint32_t voltage_ref_param;         /**< reference value to calculate actual voltage value */
    uint8_t current_io_num;             /**< gpio number of the pin outputing current */
    pcnt_unit_t current_pcnt_unit;      /**< pulse count unit of current pin */
    uint32_t current_ref_param;         /**< reference value to calculate actual current value */
    uint8_t sel_io_num;                 /**< gpio number of mode select pin */
    uint8_t sel_level;                  /**< the gpio level you wang to set to mode select pin */
    pm_mode_t pm_mode;                  /**< mode of power meter refer to struct pm_mode_t */
} pm_config_t;

typedef void* pm_handle_t;

/**
  * @brief  create power meter
  *
  * @param  pm_config refer to struct pm_config_t
  *
  * @return the handle of the power meter
  */
pm_handle_t iot_powermeter_create(pm_config_t pm_config);

/**
  * @brief  delete power meter, free memory
  *
  * @param  pm_handle handle of the power meter
  *
  * @return 
  *     - ESP_OK: succeed
  *     - ESP_FAIL: pm_handle is NULL
  */
esp_err_t iot_powermeter_delete(pm_handle_t pm_handle);

/**
  * @brief  read value of power meter
  *
  * @param  pm_handle handle of the power meter
  * @param  value_type which value you want to read, refer to struct pm_value_type_t
  *
  * @return 
  */
uint32_t iot_powermeter_read(pm_handle_t pm_handle, pm_value_type_t value_type);

/**
  * @brief  change the mode of power meter
  *
  * @param  pm_handle handle of the power meter
  * @param  mode the mode you want to change to 
  *
  * @return 
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t iot_powermeter_change_mode(pm_handle_t pm_handle, pm_mode_t mode);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
 * class of power meter
 * simple usage:
 * pm_config_t pm_conf;
 * ......(set pm_config)
 * CPowerMeter* my_pm = new CPowerMeter(pm_conf);
 * my_pm->change_mode(PM_SINGLE_VOLTAGE);
 * vTaskDelay(5000 / portTICK_RATE_MS);
 * ESP_LOGI(TAG, "value of power:%d", my_pm->read(PM_POWER));
 */
class CPowerMeter
{
private:
    pm_handle_t m_pm_handle;

    /**
     * prevent copy construct
     */
    CPowerMeter(const CPowerMeter&);
    CPowerMeter& operator = (const CPowerMeter&);

public:
    /**
     * @brief  constructor of CPowerMeter
     *
     * @param  pm_config config of power meter
     */
    CPowerMeter(const pm_config_t &pm_config);
    ~CPowerMeter();

    /**
     * @brief  get measure value of power meter
     *
     * @param  value_type refer to enum pm_value_type_t
     *
     * @return value of measurement
     */
    uint32_t read(pm_value_type_t value_type);

    /**
     * @brief  change mode of power meter
     *
     * @param  mode refer to enum pm_mode_t
     *
     * @return
     *     - ESP_OK: succeed
     *     - others: fail
     */
    esp_err_t change_mode(pm_mode_t mode);
};
#endif
#endif
#endif
