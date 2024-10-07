"""
Requires:
    - st-utils v1.6.0
    - arm-none-eabi-size
"""

import os
import colorama
import click

colorama.init()

IMAGE = "cmake_build/{APP_TARGET}.bin"
ELF_IMAGE = "cmake_build/{APP_TARGET}.elf"

APPLICATION = "./application.bin"

CMAKE_GEN_CMD = '''cmake -S . -B cmake_build/ -G Ninja'''
CMAKE_BUILD_CMD = '''cmake --build cmake_build/'''

FLASH_APPLICATION_CMD = '''st-flash write 
{image} 0x8040000 && st-info --reset
'''

def build_image():
    print(colorama.Fore.CYAN, "Building Image .." + colorama.Style.RESET_ALL)

    cmake_gen_cmd = CMAKE_GEN_CMD.format()
    cmake_build_cmd = CMAKE_BUILD_CMD.format()

    print(cmake_gen_cmd)
    print(cmake_build_cmd)

    ret = os.system(" ".join(cmake_gen_cmd.split("\n")))
    if ret != 0:
        raise Exception("CMake configuration generation failed!")

    ret = os.system(" ".join(cmake_build_cmd.split("\n")))
    if ret != 0:
        raise Exception("CMake build failed!")

    print(colorama.Fore.GREEN + "Compiled firmware successfully" + colorama.Style.RESET_ALL)

def _flash_application(image=APPLICATION):
    cmd = FLASH_APPLICATION_CMD.format(image=image)
    print(cmd)

    if not os.path.exists(image):
        raise FileNotFoundError(image)


    ret = os.system(" ".join(cmd.split("\n")))
    if ret != 0:
        msg = "Unable to flash application"
        print(colorama.Fore.RED + msg + colorama.Style.RESET_ALL)
        raise Exception(msg)
    print(colorama.Fore.GREEN + "Application flashed" + colorama.Style.RESET_ALL)

@click.group()
def cli():
    """
    Firmware build tool
    """

    print(colorama.Fore.GREEN + "[CLI]\n" + colorama.Style.RESET_ALL)
    pass

@cli.command()
def compile_image():
    """
    compiles image
    """
    build_image()

@cli.command()
def flash_application():
    """
    flashes application
    """
    _flash_application()

@cli.command()
@click.option("--compile", is_flag=True, help="compile image, default False")
@click.option("--flash", is_flag=True, help="flash application, default False")
def build_application(compile, flash):
    if compile:
        build_image()
    if flash:
        _flash_application("cmake_build/baremetal_arm.bin")

if __name__ == "__main__":
    cli()

