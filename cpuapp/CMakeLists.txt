#
# Copyright (c) 2020 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.20.0)

set(BOARD nrf5340dk_nrf5340_cpuapp)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(entropy_nrf53_cpuapp)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/entropy_ser.c)

target_include_directories(app PRIVATE ../)
target_include_directories(app PRIVATE ../../nrf_rpc_1/include)
target_include_directories(app PRIVATE ../../nrf_rpc_2/include)

set(nrf_rpc_1_base   ${CMAKE_CURRENT_SOURCE_DIR}/../nrf_rpc_1)
set(nrf_rpc_2_base   ${CMAKE_CURRENT_SOURCE_DIR}/../nrf_rpc_2)

add_subdirectory(${nrf_rpc_1_base} build/nrf_rpc_1)
add_subdirectory(${nrf_rpc_2_base} build/nrf_rpc_2)

# zephyr_compile_options(-save-temps)
