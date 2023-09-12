#include <stdio.h>
#include <iostream>

#define UART_REG(reg) ((volatile char *)(0x10000000L + reg))

using namespace std;

int main() {
    cout << 0x10000000L + 1 << endl;
    printf("%p\n", (volatile char *)(0x10000000L + 3));
    cout << (volatile char *)(0x10000000L + 3) << endl;
    cout << UART_REG(1) << endl;
    return 0;
}