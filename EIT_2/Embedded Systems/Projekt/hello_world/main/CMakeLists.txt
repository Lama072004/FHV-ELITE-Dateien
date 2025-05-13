# Minimum required CMake version
cmake_minimum_required(VERSION 3.5)

# Component definition
idf_component_register(
    SRCS "Pulsoxymetrie.c"
    PRIV_REQUIRES 
        driver
        freertos
        esp_timer
    INCLUDE_DIRS ""
)