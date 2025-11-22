file(GLOB SRC_FILES "src/*.c") # automatisch alle c-dateien erkennen und verwenden

idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS "hdr" # header-dateien
    REQUIRES bt esp_adc nvs_flash driver
)
