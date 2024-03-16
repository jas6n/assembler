/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS 

// Machine Definitions
#define MEMORYSIZE 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct 
stateStruct {
    int pc;
    int mem[MEMORYSIZE];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

static inline int convertNum(int32_t);

int 
main(int argc, char **argv)
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; ++state.numMemory) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address  %d\n. Please ensure you are providing a machine code file.", state.numMemory);
            perror("sscanf");
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    //Your code starts here!
    bool cont = true;

    state.pc = 0;

    // initialize all registers to 0
    for (int i = 0; i < NUMREGS; ++i){
        state.reg[i] = 0;
    }
    
    while (cont){

        printState(&state);

        int combine1 = 0b00000000001111111111111111111111;
        int combine2 = 0b00000000000001111111111111111111;
        int combine3 = 0b00000000000000000000000000000111;
        int combineOff = 0b1111111111111111;
//
        // check opcode
        // check if it is add
        int instruction = state.mem[state.pc];

        // add
        if ((instruction >> 22) == 0){


            int reg1 = (combine1 & instruction) >> 19; // zero out stuff not important that is before reg1 and shift it to reveal numbers
            int reg2 = (combine2 & instruction) >> 16;
            int destReg = (combine3 & instruction);

            state.reg[destReg] = state.reg[reg1] + state.reg[reg2];
            
            ++state.pc; // not entirely sure if this works but whatever
            
        }

        // nor
        else if ((instruction >> 22) == 1){

            int reg1 = (combine1 & instruction) >> 19; 
            int reg2 = (combine2 & instruction) >> 16;
            int destReg = (combine3 & instruction);

            state.reg[destReg] = ~(state.reg[reg1] | state.reg[reg2]);
            ++state.pc;
        }

        // lw
        else if ((instruction >> 22) == 2){
            

            int reg1 = (combine1 & instruction) >> 19; 
            int reg2 = (combine2 & instruction) >> 16;
            int offset = convertNum(combineOff & instruction);

            // if (offset >= 32768){
            //     offset = -(65536 - offset);
            // }

            state.reg[reg2] = state.mem[offset + state.reg[reg1]];

            ++state.pc;

        }

        // sw
        else if ((instruction >> 22) == 3){

            
            int reg1 = (combine1 & instruction) >> 19; 
            int reg2 = (combine2 & instruction) >> 16;
            int offset = convertNum(combineOff & instruction);

            // if (offset >= 32768){
            //     offset = -(65536 - offset);
            // }

            state.mem[offset + state.reg[reg1]] = state.reg[reg2];
            ++state.pc;

        }

        // beq
        else if ((instruction >> 22) == 4){

            int reg1 = (combine1 & instruction) >> 19; 
            int reg2 = (combine2 & instruction) >> 16;
            int offset = convertNum(combineOff & instruction);

            // if (offset >= 32768){
            //     offset = -(65536 - offset);
            // }

            if (state.reg[reg1] == state.reg[reg2]){
                state.pc += (1 + offset);
            } else {

                ++state.pc;
            }
            
        }

        // jalr
        else if ((instruction >> 22) == 5){


            int reg1 = (combine1 & instruction) >> 19; 
            int reg2 = (combine2 & instruction) >> 16;
            ++state.pc;

            state.reg[reg2] = state.pc;
            state.pc = state.reg[reg1];
            
        }

        // halt
        else if ((instruction >> 22) == 6){

            cont = false;
            ++state.pc;
        }

        // noop
        else if ((instruction >> 22) == 7){
            ++state.pc;

            
        }

        // ++state.pc;
    



        // print the state of the code
        

        // if (state.pc > 8){
        //     cont = false;
        // }
    }



    //Your code ends here! 
    printState(&state);
    return(0);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW. 
*/

void 
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) 
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) 
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/
