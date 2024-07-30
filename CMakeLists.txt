cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set (APP_TARGET baremetal_arm)

project(${APP_TARGET})

add_executable(${APP_TARGET})

target_include_directories(${APP_TARGET}
        PRIVATE
        include/
        include/cmsis
)

target_sources(${APP_TARGET}
        PRIVATE
        src/main.c
        src/delay.c
        src/bsp.c
        core/system_stm32f4xx.c
        startup/startup.c
)