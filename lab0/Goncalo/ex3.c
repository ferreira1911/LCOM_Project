#include <stdlib.h>
#include <stdio.h>

int numberOfBits(int number);
void numberInBits(int number);
void foo();

/////////////////////////////////////////
// Challenge 1

void numberInBits(int number) {
    int numOfBits = numberOfBits(number);
    printf("Number %d in Bits: ", number);
    for(int i = numOfBits; i > 0; i--){
        int num = (number >> (i - 1)) & 1;
        printf("%d", num);
    }
    printf("\n");
}

int numberOfBits(int number){
    int count = 0;
    while(number > 0){
        number = number >> 1;
        count++;
    }
    return count;
}

/////////////////////////////////////////

void foo() {
    printf("Bit 3 está definido!\n");
}

int main() {
    
    //Operações
    int n = 0;
    printf("n=%d\n", n);
    n = n | 1; // OR
    printf("n|1=%d\n", n);
    

    printf("0b101^0b10=%d\n", 0b101^0b10); // XOR(1 quando são diferentes)
    printf("0xF+1=%d\n", 0xF+1); // 0xF(hexadecimal) = 15(decimal) 

    // Manipulação de Bits
    int k = 0xE;
    printf("k=%d\n", k); // 0xE = 0b1110
    printf("Bit 3 of k is %d\n", (k >> 3) & 1);

    // Set de um bit
    k |= (1 << 3); // k = k | (1 << 3)
    printf("k=%d\n", k);

    // Clear de um bit
    k &= ~(1 << 3); // k = k & ~(1 << 3)
    printf("k=%d\n", k);

    // Uso de Macros
    #define BIT(n) (1 << (n)) // () prevent precedence issues
    k |= BIT(3); // set bit 3
    k &= ~BIT(3); // clear bit 3
    if (k & BIT(3)) foo(); // check if bit 3 is set

    // Challenge 1:
    int number = 10;
    numberInBits(number);


    n = -1;
    printf("n=%d\n", n);
    printf("~n=%d\n", ~n);
    printf("n>>1=%d\n", n>>1);
    printf("n<<1=%d\n", n<<1);
    printf("(((unsigned int)n)>>1=%d\n", (((unsigned int)n)>>1));
    printf("(((unsigned int)n)<<1=%d\n", (((unsigned int)n)<<1));

    return 0;
}
