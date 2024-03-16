/******************************************************************************

                            Online C Debugger.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Debug" button to debug program.

*******************************************************************************/

// Online C compiler to run C program online
#include <stdio.h>

int main() {
    // Write C code here
    int mult1 = 6203;
    int mult2 = 1429;
    int bit = 0;
    int answer = 0;
    int shift = 1;
    int counter = 0;
    
    while (mult2 != 0){
        bit = mult2 & shift; 
        if (bit == shift){ 
            answer += (mult1 << counter);
        } else {
            
        }
        shift = shift << 1;
        counter += 1;
        mult2 = mult2 & (0b1111111111111111 << counter);
        // printf("%d", mult2);
        // printf("\n");
    }
    printf("%d", answer);

    return 0;
}
