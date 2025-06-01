idf_component_register(
    SRCS "main.c"
         "sensor_init.c"
         "sensor_read_write.c"
         "heart.c"
         "ble.c"
    INCLUDE_DIRS "."
    REQUIRES driver 
             bt 
             nvs_flash 
             esp_common
             esp_system
             freertos
             log
)