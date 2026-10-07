# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# Bluetooth Classic aktivieren (SPP benötigt)
set(CONFIG_BT_ENABLED 1)
set(CONFIG_BT_BLUEDROID_ENABLED 1)
set(CONFIG_BT_CLASSIC_ENABLED 1)
set(CONFIG_BT_SPP_ENABLED 1)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Bluetooth-test-minimal)
