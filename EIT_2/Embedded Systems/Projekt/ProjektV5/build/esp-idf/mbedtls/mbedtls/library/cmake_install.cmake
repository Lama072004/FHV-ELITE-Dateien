# Install script for directory: C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/library

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/ProjektV5")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/Debug/libmbedcrypto.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/Release/libmbedcrypto.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/MinSizeRel/libmbedcrypto.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/RelWithDebInfo/libmbedcrypto.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/Debug/libmbedx509.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/Release/libmbedx509.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/MinSizeRel/libmbedx509.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/RelWithDebInfo/libmbedx509.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/Debug/libmbedtls.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/Release/libmbedtls.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/MinSizeRel/libmbedtls.a")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Mathias Lampert/Documents/Mathias Schule_Studium/FH/ELITE-Semester/EIT_2/Embedded Systems/Projekt/ProjektV5/build/esp-idf/mbedtls/mbedtls/library/RelWithDebInfo/libmbedtls.a")
  endif()
endif()

