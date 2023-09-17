# INSTRUCTIONS

## BUILD

```console
rm -rf build/
mkdir build/
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4 -mthumb -c -o build/startup.o startup/startup.c 
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4 -mthumb -c -o build/bsp.o src/bsp.c 
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4 -mthumb -c -o build/delay.o src/delay.c 
arm-none-eabi-gcc -O0 -g -Wall -mcpu=cortex-m4 -mthumb -c -o build/main.o src/main.c 
arm-none-eabi-ld -Map build/main.map -T startup/linker.ld -o build/main.elf build/startup.o build/bsp.o build/delay.o build/main.o
```
## FLASH

```console
arm-none-eabi-objcopy -O binary build/main.elf build/main.bin
st-flash write build/main.bin 0x08000000
```

