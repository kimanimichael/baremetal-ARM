cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set (APP_TARGET baremetal_arm)

set(CMAKE_ASM_COMPILER "arm-none-eabi-as")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_LINKER "arm-none-eabi-ld")

add_executable(${APP_TARGET})

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/startup/linker.ld")
set_target_properties(${APP_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -march=armv7e-m -Wall -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

target_compile_options(baremetal_arm PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -march=armv7e-m
        -Wall
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        --specs=nano.specs
)

target_link_options(baremetal_arm PRIVATE
        "-T${LINKER_SCRIPT}"
        "-nostartfiles"
        "-specs=nano.specs"
        "-specs=nosys.specs"
        ${CPU_FLAGS}

)


set(CMAKE_C_FLAGS "${CPU_FLAGS} -O0 -g -Wdiscarded-qualifiers -nostdlib")
set(CMAKE_CXX_FLAGS "${CPU_FLAGS} -O0 -g -Wdiscarded-qualifiers -nostdlib")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")

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
        -Wl,--start-group
        -lc_nano           # the nano-libc implementation
        -lnosys            # stub syscalls (sbrk/write/etc)
        -lgcc              # compiler’s helper routines
        -Wl,--end-group
)

target_include_directories(${APP_TARGET}
        PRIVATE
        include/
        include/cmsis
        include/cmsis/m-profile
)

target_sources(${APP_TARGET}
        PRIVATE
        src/main.c
        src/delay.c
        src/bsp.c
        src/shape.c
        src/rectangle.c
        src/syscalls.c
        core/system_stm32f4xx.c
        startup/startup.c

)

# Post-build step to generate binary
add_custom_command(TARGET ${APP_TARGET} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/${APP_TARGET} ${CMAKE_BINARY_DIR}/${APP_TARGET}.bin
)

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

