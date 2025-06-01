# Install script for directory: C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ProjektV5")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/idf3tools/tools/riscv32-esp-elf/esp-14.2.0_20241119/riscv32-esp-elf/bin/riscv32-esp-elf-objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/block_cipher.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/build_info.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_dependencies.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "C:/idf3/v5.4.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

