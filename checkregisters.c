/*Matricola = 876335, Nome Gruppo = P1G66*/
#include <stdio.h>
#include <stdlib.h>
#include "checkregisters.h"
/*Funzione che controlla che i registri non vadano oltre i limiti predisposti,
ritornando un errore e interrompendo il programma altrimenti. Viene richiamata dalle
funzioni 'execute' e 'print'.*/
void check_valid_registers(int i, int dim){
    if (i<0 || i>=dim){
        printf("Error: attempted access to invalid register: R%d.\n", i);
        exit(1);
    }
}
