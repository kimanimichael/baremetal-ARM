cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set (APP_TARGET baremetal_arm)

set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_LINKER "arm-none-eabi-ld")

add_executable(${APP_TARGET})

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/startup/linker.ld")
set_target_properties(${APP_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_options(baremetal_arm PRIVATE "-T${LINKER_SCRIPT}"
        "-nostartfiles")

set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Wall")
set(CMAKE_C_FLAGS "${CPU_FLAGS} -g -Werror -Wdiscarded-qualifiers")
#set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/startup/linker.ld -Wl,-Map=${CMAKE_BINARY_DIR}/${APP_TARGET}.map ${CPU_FLAGS} --specs=nosys.specs" CACHE INTERNAL "")

set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/startup/linker.ld")

set(QPC_CFG_KERNEL qxk)
set(QPC_CFG_PORT arm-cm)
set(QPC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qpc CACHE INTERNAL "")
add_subdirectory(${QPC_PATH})

project(${APP_TARGET})

set_target_properties(${APP_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_libraries(${APP_TARGET}
    PRIVATE
        qpc
)

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
        src/shape.c
        src/rectangle.c
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

