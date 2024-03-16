/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
// int get_index(char * label, struct Labels * ls);


struct Labels {
    char name[MAXLINELENGTH];
    int index;
};


int
main(int argc, char **argv)
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 4) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        /* reached end of file */
    }


    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    struct Labels ls[MAXLINELENGTH];
    int ind = 0;
    int index = 0;


    // first pass - creates an array with a Labels struct containing the label name and its index
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if (strcmp(label, "")) {
            ls[ind].index = index;
            strcpy(ls[ind].name, label);
            ind += 1;
        }
        index += 1;
    }

    // check for duplicate labels
    for (int i = 0; i < ind; ++i){
        int count = 0;
        for (int j = 0; j < ind; ++j){
            if(!strcmp(ls[i].name, ls[j].name)){
                count += 1;
            }   
        }
        if (count > 1){
            printf("error");
            exit(1);
        }
    }

    rewind(inFilePtr);


    // write to file
    // int zzz = 0;

    int line = 0;

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){

        bool rtype = false;
        bool itype = false;
        bool jtype = false;
        bool otype = false;

        // handle .fill
        if (!strcmp(opcode, ".fill")){
            if (isNumber(arg0)){
                fprintf(outFilePtr, "%d", atoi(arg0));
                fprintf(outFilePtr, "\n");
            } else {

                int count = 0;
                for (int j = 0; j < ind; ++j){
                    if(!strcmp(arg0, ls[j].name)){
                        count += 1;
                    }   
                }
                if (count != 1){
                    printf("no label error");
                    exit(1);
                }


                int k;
                int z = 0;
                    while (strcmp(arg0, ls[z].name)){
                        z += 1;
                    }

                    k = ls[z].index;
                
                fprintf(outFilePtr, "%d", k);
                fprintf(outFilePtr, "\n");
            }

        } else {

            
            // set first part of bits to be 0s
            unsigned int assembled = 0;
            


            // add opcode 
            if (!strcmp(opcode, "add")){
                rtype = true;
            } else if (!strcmp(opcode, "nor")){
                assembled = (assembled | 1);
                rtype = true;
            } else if (!strcmp(opcode, "lw")){
                assembled = (assembled | 2);
                itype = true;
            } else if (!strcmp(opcode, "sw")){
                assembled = (assembled | 3);
                itype = true;
            } else if (!strcmp(opcode, "beq")){ // label should equate to value needed to get back to label -> label index = index + 1 - equate
                assembled = (assembled | 4);
                itype = true;
            } else if (!strcmp(opcode, "jalr")){
                assembled = (assembled | 5);
                jtype = true;
            } else if (!strcmp(opcode, "halt")){
                assembled = (assembled | 6);
                otype = true;
            } else if (!strcmp(opcode, "noop")){
                assembled = (assembled | 7);
                otype = true;
            }  else {
                printf("error opcode");
                exit(1);
            }

            // shift to the left by 22 bits - might not need to do
            // assembled = assembled << 22;

            if (rtype){

                if (isNumber(arg0) && isNumber(arg1) && isNumber(arg2)){
                    if ((atoi(arg0)>=0 && atoi(arg0)<=7) && (atoi(arg1)>=0 && atoi(arg1)<=7) && (atoi(arg2)>=0 && atoi(arg2)<=7)){

                        assembled = (assembled << 3);
                        assembled = (assembled | atoi(arg0));
                        assembled = (assembled << 3);
                        assembled = (assembled | atoi(arg1));
                        assembled = (assembled << 16);
                        assembled = (assembled | atoi(arg2));
                    } else{
                        printf("error");
                        exit(1);
                    }
                } else {
                    printf("error");
                    exit(1);
                }

            

            } else if (itype){

                if (isNumber(arg0) && isNumber(arg1)){
                    if ((atoi(arg0)>=0 && atoi(arg0)<=7) && (atoi(arg1)>=0 && atoi(arg1)<=7)){

                    } else{
                        printf("error");
                        exit(1);
                    }
                } else {
                    printf("error");
                    exit(1);
                }

                assembled = (assembled << 3);
                assembled = (assembled | atoi(arg0));
                assembled = (assembled << 3);
                assembled = (assembled | atoi(arg1));
                 // check if third field is a number
                if (isNumber(arg2)){


                    assembled = (assembled << 16);
                    if (atoi(arg2) < -32768 || atoi(arg2) > 32767){
                            exit(1);
                        }
                    assembled = (assembled | atoi(arg2));
                } else {
                    
                    // check if label is in ls

                    int count = 0;
                    for (int j = 0; j < ind; ++j){
                        if(!strcmp(arg2, ls[j].name)){
                            count += 1;
                        }   
                    }
                    if (count != 1){
                        printf("no label error");
                        exit(1);
                    }


                    if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw")){

                        
                        int m = 0b00000000000000001111111111111111;
                        int z = 0;
                        int offset = 0;
                        // index in array
                        while (strcmp(arg2, ls[z].name)){
                            z += 1;
                        }

                        // check index at array name
                        offset = ls[z].index;

                        if (offset < -32768 || offset > 32767){
                            exit(1);
                        }

                        offset = (offset & m);

                        assembled = (assembled << 16);
                        assembled = (assembled | offset);


                    } else if (!strcmp(opcode, "beq")){
                        int m = 0b00000000000000001111111111111111;
                        int z = 0;
                        int offset = 0;
                        // index in array
                        while (strcmp(arg2, ls[z].name)){
                            z += 1;
                        }

                        // check index at array name
                        offset = ls[z].index;

                        offset = offset - line - 1;

                        if (offset < -32768 || offset > 32767){
                            exit(1);
                        }



                        offset = (offset & m);

                        assembled = (assembled << 16);
                        assembled = (assembled | offset);


                    }
                }
                
            } else if (jtype){

                if (isNumber(arg0) && isNumber(arg1)){
                    if ((atoi(arg0)>=0 && atoi(arg0)<=7) && (atoi(arg1)>=0 && atoi(arg1)<=7)){

                    } else{
                        printf("error");
                        exit(1);
                    }
                } else {
                    printf("error");
                    exit(1);
                }

                assembled = (assembled << 3);
                assembled = (assembled | atoi(arg0));
                assembled = (assembled << 3);
                assembled = (assembled | atoi(arg1));
                assembled = (assembled << 16);
                
            } else if (otype){
                assembled = (assembled << 3);
                assembled = (assembled | atoi(arg0));
                assembled = (assembled << 19);


            }

            fprintf(outFilePtr, "%d", assembled);
            fprintf(outFilePtr, "\n");

            // if there isn't a label
            // if (strcmp(opcode, "noop") && strcmp(opcode, "halt")){
            // if (strcmp(opcode, "noop") || strcmp(opcode, "halt"))

        
                // }
        }
        line += 1;

    }

    




    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    // int test = 4;

    // test = test << 22;

    // test = test | 65533;


    return(0);
}


// Returns non-zero if the line contains only whitespace.
static int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                line_char_is_whitespace = 1;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            nonempty_line = 1;
            break;
        }
    }
    return !nonempty_line;
}

// Exits 2 if file contains an empty line anywhere other than at the end of the file.
// Note calling this function rewinds inFilePtr.
static void checkForBlankLinesInCode(FILE *inFilePtr) {
    char line[MAXLINELENGTH];
    int blank_line_encountered = 0;
    int address_of_blank_line = 0;
    rewind(inFilePtr);

    for(int address = 0; fgets(line, MAXLINELENGTH, inFilePtr) != NULL; ++address) {
        // Check for line too long
        if (strlen(line) >= MAXLINELENGTH-1) {
            printf("error: line too long\n");
            exit(1);
        }

        // Check for blank line.
        if(lineIsBlank(line)) {
            if(!blank_line_encountered) {
                blank_line_encountered = 1;
                address_of_blank_line = address;
            }
        } else {
            if(blank_line_encountered) {
                printf("Invalid Assembly: Empty line at address %d\n", address_of_blank_line);
                exit(2);
            }
        }
    }
    rewind(inFilePtr);
}


/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}


// int get_index(char * label, struct Labels * ls){
//     int k;
//     int z = 0;
//     while (strcmp(label, ls[z].name)){
//         z += 1;
//     }

//     k = ls[z].index;

//     return k;

// }