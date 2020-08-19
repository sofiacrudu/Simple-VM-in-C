#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "vm.h"

/*Nel main vengono richiamate le singole funzioni impementate nei file machinecode.c, execute.c e print.c,
gestisce eventuali commandi non corretti dati dalla linea di commando, accetta quindi solo due parametri:
operazione di 'esegui' o 'stampa' e nome del file di testo contenente il programma.*/
int main (int argc, char **argv){
    char *filename=argv[2];
    int *program=NULL;
    int instruction_count;
    if (argc <= 2) {
        printf("Error: no input files.\n");
        return 0;
    }
    if (argc > 3) {
        printf("Error: too many arguments supplied.\n");
        return 0;
    }
    program=machine_code(filename, &instruction_count);
    if (program==NULL){
        printf("Error: unseccessful malloc for program.\n");
        return 0;
    }
    else{
        if ((!strcmp(argv[1], "esegui")) && (argv[2]==filename) && (argc == 3)){/*si accettano solo questi parametri per eseguire il programma*/
            execute(program, instruction_count);
        }
        else if ((!strcmp(argv[1], "stampa")) && (argv[2]==filename) && (argc == 3)){/*si accettano solo questi parametri per stampare il programma*/
            print(program, instruction_count);
        }
        else
            printf("Command not found.\n");
    }
    free(program);
    return 0;
}
