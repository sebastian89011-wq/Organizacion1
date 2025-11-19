#include <stdio.h>

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main(void) {
    int x = 5;               
    int f = factorial(x);    
    printf("factorial(%d) = %d\n", x, f);
    return 0;
}
