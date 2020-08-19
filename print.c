#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "checkregisters.h"
#include "vm.h"

/*Dichiaro due array bidimensionali di tipo char contenenti le stringhe
delle istruzioni e dei registri da stampare nella funzione 'print'.*/
char *instruction_set[]={"HALT", "DISPLAY", "PRINT_STACK", "NOP3", "NOP4", "NOP5", "NOP6", "NOP7", "NOP8", "NOP9",
"PUSH", "POP", "MOV", "NOP13", "NOP14", "NOP15", "NOP16", "NOP17", "NOP18", "NOP19", "CALL", "RET", "JMP",
"JZ", "JPOS", "JNEG", "NOP26", "NOP27", "NOP28", "NOP29", "ADD", "SUB", "MUL", "DIV"};

char *regs[]={"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10",
"R11", "R12", "R13", "R14", "R15", "R16", "R17", "R18", "R19", "R20", "R21", "R22", "R23",
"R24", "R25", "R26", "R27", "R28", "R29", "R30", "R31"};

/*La funzione 'print' effettua la stampa delle singole istruzioni dell'array di interi ricevuto in input,
secondo il formato '[posizione] istruzione P1 P2', gestendo eventuali errori come ad esempio l'accesso a un
registro fuori da quelli predisposti per l'utilizzo (>=0 e <32) o la lettura di un'istruzione sconosciuta.*/
void print(int *program, int instruction_count){
    unsigned int IP = 0;
    unsigned int supp=(unsigned int)instruction_count;
    while (IP<supp){
        if (program[IP]==HALT){
            printf("[%d] %s\n", IP, instruction_set[program[IP]]);
            IP++;
        }
        else if (program[IP]==DISPLAY){
            check_valid_registers(program[IP+1], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s\n", regs[program[++IP]]);
            IP++;
        }
        else if (program[IP]==PRINT_STACK){
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%d\n", program[++IP]);
            IP++;
        }
        else if (program[IP]==PUSH){
            check_valid_registers(program[IP+1], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s\n", regs[program[++IP]]);
            IP++;
        }
        else if (program[IP]==POP){
            check_valid_registers(program[IP+1], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s\n", regs[program[++IP]]);
            IP++;
        }
        else if (program[IP]==MOV){
            check_valid_registers(program[IP+1], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s ", regs[program[++IP]]);
            printf("%d\n", program[++IP]);
            IP++;
        }
        else if (program[IP]==CALL){
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%d\n", program[++IP]);
            IP++;
        }
        else if (program[IP]==RET){
            printf("[%d] %s\n", IP, instruction_set[program[IP]]);
            IP++;
        }
        else if (program[IP]==JMP){
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%d\n", program[++IP]);
            IP++;
        }
        else if (program[IP]==JZ){
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%d\n", program[++IP]);
            IP++;
        }
        else if (program[IP]==JPOS){
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%d\n", program[++IP]);
            IP++;
        }
        else if (program[IP]==JNEG){
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%d\n", program[++IP]);
            IP++;
        }
        else if (program[IP]==ADD){
            check_valid_registers(program[IP+1], NUM_REGS);
            check_valid_registers(program[IP+2], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s ", regs[program[++IP]]);
            printf("%s\n", regs[program[++IP]]);
            IP++;
        }
        else if (program[IP]==SUB){
            check_valid_registers(program[IP+1], NUM_REGS);
            check_valid_registers(program[IP+2], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s ", regs[program[++IP]]);
            printf("%s\n", regs[program[++IP]]);
            IP++;
        }
        else if (program[IP]==MUL){
            check_valid_registers(program[IP+1], NUM_REGS);
            check_valid_registers(program[IP+2], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s ", regs[program[++IP]]);
            printf("%s\n", regs[program[++IP]]);
            IP++;
        }
        else if (program[IP]==DIV){
            check_valid_registers(program[IP+1], NUM_REGS);
            check_valid_registers(program[IP+2], NUM_REGS);
            printf("[%d] %s ", IP, instruction_set[program[IP]]);
            printf("%s ", regs[program[++IP]]);
            printf("%s\n", regs[program[++IP]]);
            IP++;
        }
        else{
            printf("UNKNOWN INSTRUCTION: %d\n", program[IP]);
            exit(1);
        }
    }
}
