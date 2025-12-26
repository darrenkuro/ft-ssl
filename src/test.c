#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint32_t x = 0x01020304;
    uint8_t *b = (uint8_t *)&x;

    printf("%02x %02x %02x %02x\n", b[0], b[1], b[2], b[3]);
}
