file(GLOB_RECURSE SRC_FILES "src/*.c")

idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS "hdr"
    REQUIRES nvs_flash lvgl esp_lvgl_port esp_lcd esp_driver_rmt
)

