arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4 -mthumb -c -o delay.o delay.c 
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4 -mthumb -c -o main.o main.c 
arm-none-eabi-ld -Map main.map -T linker.ld -o main.elf delay.o main.o
# arm-none-eabi-ld -T linker.ld -o main.elf delay.o main.o
arm-none-eabi-objcopy -O binary main.elf main.bin
st-flash write main.bin 0x08000000