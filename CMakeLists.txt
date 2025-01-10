cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set (APP_TARGET baremetal_arm)

set(CMAKE_ASM_COMPILER "arm-none-eabi-as")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_LINKER "arm-none-eabi-ld")

add_executable(${APP_TARGET})

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/startup/linker.ld")
set_target_properties(${APP_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_options(baremetal_arm PRIVATE "-T${LINKER_SCRIPT}"
        "-nostartfiles")

set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -Wall -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CPU_FLAGS} -O0 -g -Werror -Wdiscarded-qualifiers -fstack-usage -nostdlib -specs=nano.specs")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/startup/linker.ld -Wl,-Map=${CMAKE_BINARY_DIR}/${APP_TARGET}.map ${CPU_FLAGS} --specs=nosys.specs" CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/startup/linker.ld")

set(QPC_CFG_KERNEL qxk)
set(QPC_CFG_PORT arm-cm)
set(QPC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qpc CACHE INTERNAL "")
add_subdirectory(${QPC_PATH})

project(${APP_TARGET})
enable_language(ASM)


set_target_properties(${APP_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_libraries(${APP_TARGET}
    PRIVATE
        qpc
)

target_include_directories(${APP_TARGET}
        PRIVATE
        include/
        include/cmsis
        include/cmsis/m-profile
        qpc/3rd_party/uC-OS2/Source
        qpc/3rd_party/uC-OS2/Cfg/Template
        qpc/3rd_party/uC-OS2/Ports/ARM-Cortex-M/ARMv7-M/ARM
)

target_sources(${APP_TARGET}
        PRIVATE
        src/main.c
        src/delay.c
        src/bsp.c
        src/shape.c
        src/rectangle.c
        core/system_stm32f4xx.c
        startup/startup.c
        qpc/3rd_party/uC-OS2/Source/os_core.c
        qpc/3rd_party/uC-OS2/Source/os_flag.c
        qpc/3rd_party/uC-OS2/Source/os_mbox.c
        qpc/3rd_party/uC-OS2/Source/os_mem.c
        qpc/3rd_party/uC-OS2/Source/os_mutex.c
        qpc/3rd_party/uC-OS2/Source/os_q.c
        qpc/3rd_party/uC-OS2/Source/os_sem.c
        qpc/3rd_party/uC-OS2/Source/os_task.c
        qpc/3rd_party/uC-OS2/Source/os_time.c
        qpc/3rd_party/uC-OS2/Source/os_tmr.c
        qpc/3rd_party/uC-OS2/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.s
        qpc/3rd_party/uC-OS2/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.c
        qpc/3rd_party/uC-OS2/Ports/ARM-Cortex-M/ARMv7-M/os_cpu_c.c
)

# Post-build step to generate binary
add_custom_command(TARGET ${APP_TARGET} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/${APP_TARGET} ${CMAKE_BINARY_DIR}/${APP_TARGET}.bin
)

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

