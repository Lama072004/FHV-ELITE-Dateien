idf_component_register(
    SRCS "main.c"
    
    INCLUDE_DIRS "."
    REQUIRES driver 
             bt 
             nvs_flash 
             esp_common
             esp_system
             freertos
             log
)