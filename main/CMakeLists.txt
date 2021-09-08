set(SOURCES main.c)
idf_component_register(SRC_DIRS "." "images" "sounds"
                    INCLUDE_DIRS "includes" "corehttp/include" 
                    "corehttp/interface"  "Common"
                    "../../../freertos/FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/include" 
                    "../../../freertos/FreeRTOS/FreeRTOS-Plus/Source/Utilities/backoff_algorithm/source/include/"
                    "../../../freertos/FreeRTOS/FreeRTOS/Source/include" 
                    "../../../freertos/FreeRTOS/FreeRTOS/Test/CBMC/patches"                    
                    "../.pio/libdeps/core2foraws/FreeRTOS/src"                  
                    "../.pio/libdeps/core2foraws/Adafruit SGP30 Sensor"                   
                    REQUIRES "core2forAWS" "esp-cryptoauthlib" "fft" "nvs_flash")
