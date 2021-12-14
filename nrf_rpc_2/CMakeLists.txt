#
#
#

target_sources(app PRIVATE nrf_rpc_os.c)
target_sources(app PRIVATE nrf_rpc_rpmsg.c)

target_include_directories(app PRIVATE include)
target_include_directories(app PRIVATE ../nrf_rpc_1/include)