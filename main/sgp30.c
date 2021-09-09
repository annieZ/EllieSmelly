#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"

#include "core2forAWS.h"
#include "global.h"
#include "keyboard.h"
#include "received.h"
#include "core_http_config.h"
#include "i2c_device.h"
#include "cta.h"

I2CDevice_t i2c_device;

#define DEVICE_ADDRESS 0xA0 >> 1

static const char* TAG = CTA_TAB_NAME;
void aws_sgp30_task(void *param) {

    //PORT_A_SDA_PIN (same as GPIO 32) for the I2C data pin and
    // PORT_A_SCL_PIN (same as GPIO 33) for the I2C clock line pin. 
    //Peripherals attached to expansion port A 
    // initialize port
   
    esp_err_t err_sda = Core2ForAWS_Port_PinMode(PORT_A_SDA_PIN, I2C);
    
    esp_err_t err_scl = Core2ForAWS_Port_PinMode(PORT_A_SCL_PIN, I2C);
    ESP_LOGI(TAG, "Status of setting the sda pin: %d", err_sda);
    ESP_LOGI(TAG, "Status of setting the scl pin: %d", err_scl);
     I2CDevice_t port_A_peripheral = Core2ForAWS_Port_A_I2C_Begin(DEVICE_ADDRESS, PORT_A_I2C_STANDARD_BAUD);
    
    vTaskDelay(pdMS_TO_TICKS(10000));
    for(;;){
        uint8_t data = 255;
        esp_err_t err = Core2ForAWS_Port_A_I2C_Read(port_A_peripheral, I2C_NO_REG, &data, 1);
        if(!err){
            ESP_LOGI(TAG, "data — %ubpm", data);
            tvoc = (const char *) &data;
            eCO2 = (const char *)&data;
            
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
    Core2ForAWS_Port_A_I2C_Close(port_A_peripheral);
    vTaskDelete(NULL);

   
}