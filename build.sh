rm -rf build/
mkdir build/
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4+nofp -mthumb -c -o build/startup.o startup/startup.c
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4+nofp -mthumb -c -o build/bsp.o src/bsp.c
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4+nofp -mthumb -c -o build/delay.o src/delay.c
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4+nofp -mthumb -c -o build/system_stm32f4xx.o core/system_stm32f4xx.c
#arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4+nofp -mthumb -c -o build/miros.o core/miros/miros.c
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4+nofp -mthumb -c -o build/main.o src/main.c
# arm-none-eabi-ld -Map build/main.map -T startup/linker.ld -o build/main.elf build/startup.o build/bsp.o build/delay.o build/system_stm32f4xx.o build/main.o

arm-none-eabi-ld -Map build/main.map -T startup/linker.ld -o build/main.elf build/startup.o build/bsp.o build/delay.o build/main.o
arm-none-eabi-ld -Map build/main.map -T startup/linker.ld -o main.elf build/startup.o build/bsp.o build/delay.o build/main.o


# arm-none-eabi-ld -Map build/main.map -T startup/linker.ld -o build/main.elf build/startup.o build/bsp.o build/delay.o build/miros.o build/system_stm32f4xx.o build/main.o
# arm-none-eabi-ld -Map build/main.map -T startup/linker.ld -o main.elf build/startup.o build/bsp.o build/delay.o build/miros.o build/system_stm32f4xx.o build/main.o


# arm-none-eabi-ld -T linker.ld -o main.elf delay.o main.o
arm-none-eabi-objcopy -O binary build/main.elf build/main.bin