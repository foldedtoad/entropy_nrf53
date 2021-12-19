# entropy_nrf53
experiments with RPC on nRF5340
This is a learning example, and is probably too simple for direct use.

## Overview
The basis of this project came from Nordic's [nRF Connect SDK](https://github.com/nrfconnect).
The various RPC libraries and example code are found in several of these projects.
This project gathers all these component into one place.
The build system has also be changed to be 'pure' Zephyr based.

## Dependencies
* Nordic nRf5340 board (PCA10095)
* Zephyr V2.7

## Building
The project consists of two seperate build elements: Master-side and Remote-side.  
There Master-side subproject is held in the "cpuapp" directory, while the Remote-side is in the "cpunet" directory.  

It is suggested that you have two terminal shells, each with established Zephyr build environments.
On shell for "cpuapp" builds and the other for "cpunet" builds.


### CMake Configuring of the Master Side  (build shell "cpuapp")
```
> cd cpuapp
> ./configure.sh
> cd build
```
### CMake Configuring of the Remote Side  (build shell "cpunet")
```
> cd cpunet
> ./configure.sh
> cd build
```
### Build-time Configuration Options
There are various build-time options which can be modified.
These options can be modified with `make menuconfig` or the equivelant.  
Foe example for "cpuapp" shell.
```
> cd cpuapp/build
> make menuconfig
```
### Building "cpuapp' Firmware  (build shell "cpuapp")
From the 'cpuapp\build' directory, do the following
```
> make
```

### Building "cpunet" Firmware (build shell "cpunet")
From the 'cpunet\build' directory, do the following
```
> make
```

### Flashing the Firmware 
Insure that the nRF5340 (PCA10095) board is connected by USB cable.  
Do the following in both build shells.
```
> make flash
```

## View the Output
To see the output, you will need to start two serial terminal shells (minicom or putty).
The nRF5340 board will create three USB-serial ports on your host system.
Usually the first and third are used for displaying output.

### Output from "cpuapp" Side (Master side)

```
*** Booting Zephyr OS build v2.7.99-1576-gdb030c24b3a8  ***
[00:00:00.254,272] <inf> app_entropy_ser: Init begin.
[00:00:00.254,333] <inf> NRF_RPC_TR: RPC Master
[00:00:00.256,591] <inf> NRF_RPC_TR: Handshake done
[00:00:00.256,652] <inf> app_entropy_ser: Init done.
[00:00:00.261,871] <inf> app_main: Entropy sample started [APP Core].
[00:00:02.262,786] <inf> app_main: entropy_remote_get
                                   4f e8 69 3e ef 96 3e 9e  79 8e                   |O.i>..>. y.      
[00:00:04.263,427] <inf> app_main: entropy_remote_get_inline
                                   cc 63 ff 09 6f ac a6 ce  be 44                   |.c..o... .D      
[00:00:06.264,099] <inf> app_main: entropy_remote_get_async
                                   17 9d f0 52 82 f4 9d 98  de 85                   |...R.... ..      
[00:00:08.264,190] <inf> app_main: entropy_remote_get_async
                                   d0 8a 0d d1 db 6a 1a 77  55 c6                   |.....j.w U.      
[00:00:10.265,045] <inf> app_main: entropy_remote_get
                                   91 3e 0c 5f b4 64 ad c6  b6 83                   |.>._.d.. ..      
[00:00:12.265,716] <inf> app_main: entropy_remote_get_inline
                                   3e 85 8e 7f e4 9b 58 84  3b 7d                   |>.....X. ;}      
[00:00:14.266,357] <inf> app_main: entropy_remote_get_async
                                   5a 80 57 9c ca 20 d4 97  ac 15                   |Z.W.. .. ..      
[00:00:16.266,418] <inf> app_main: entropy_remote_get_async
                                   dd 0f ca b6 1f 54 64 c4  e2 c7                   |.....Td. ..      
[00:00:18.267,303] <inf> app_main: entropy_remote_get
                                   a5 83 61 f3 e1 a3 6b 72  38 e2                   |..a...kr 8.      
```

### Output from "cpunet" Side (Remote side)
```
*** Booting Zephyr OS build v2.7.99-1576-gdb030c24b3a8  ***
[00:00:00.000,732] <inf> net_entropy_ser: Init begin.
[00:00:00.000,823] <inf> NRF_RPC_TR: RPC Remote
[00:00:00.000,946] <inf> NRF_RPC_TR: Handshake done
[00:00:00.001,129] <inf> net_entropy_ser: Init done.
[00:00:00.006,408] <inf> net_main: Entropy sample started [NET Core].
```
