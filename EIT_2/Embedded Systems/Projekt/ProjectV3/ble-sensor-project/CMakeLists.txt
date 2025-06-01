project(ble-sensor-project VERSION 1.0)

set(CMAKE_C_STANDARD 11)

include_directories(main)

file(GLOB SOURCES
    main/ble.c
    main/heart.c
    main/main.c
)

add_executable(ble-sensor-project ${SOURCES})

target_link_libraries(ble-sensor-project
    PRIVATE
    esp32
    nvs_flash
    freertos
    esp_bt
    esp_gap_ble
    esp_gatts
    esp_gatt_common
)