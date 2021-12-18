#
#
#

target_sources(app PRIVATE nrf_rpc.c)
target_sources(app PRIVATE nrf_rpc_cbor.c)

target_include_directories(app PRIVATE include)
target_include_directories(app PRIVATE ../nrf_rpc_2/include)

zephyr_linker_sources(SECTIONS nrf_rpc.ld)

# zephyr_compile_options(-save-temps)
