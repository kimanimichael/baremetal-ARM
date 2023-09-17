#include "../include/defs.h"
#include "../include/cmsis/stm32f429xx.h"

void assert_failed(char const* file, int line) {
    /**
     * @brief Resets the syste,
    */
    NVIC_SystemReset();
}
