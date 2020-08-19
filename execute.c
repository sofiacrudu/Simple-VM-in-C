#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "checkregisters.h"
#include "vm.h"

/*Le successive quattro funzioni servono a gestire le operazioni aritmetiche, in modo tale
che in caso di overflow numerico il programma termini stampando l'errore in uscita dal programma.*/
int safe_add(int a, int b) {
    int res=0;
    if (a>0 && b>INT_MAX-a){
        printf("Error: numeric overflow in ADD.\n");
        exit(1);
    }
    if (a<0 && b<INT_MIN-a){
        printf("Error: numeric overflow in ADD.\n");
        exit(1);
    }
    return res=a+b;
}

int safe_sub(int a, int b){
    int res=0;
    if (a<0 && b>INT_MAX+a){
        printf("Error: numeric overflow in SUB.\n");
        exit(1);
    }
    if (a>0 && b<INT_MIN+a){
        printf("Error: numeric overflow in SUB.\n");
        exit(1);
    }
    return res=a-b;
}

int safe_mul(int a, int b){
    int res=0;
    if (a==-1 && b==INT_MIN){
        printf("Error: numeric overflow in MUL.\n");
        exit(1);
    }
    if (b==-1 && a==INT_MIN){
        printf("Error: numeric overflow in MUL.\n");
        exit(1);
    }
    if (a>INT_MAX/b){
        printf("Error: numeric overflow in MUL.\n");
        exit(1);
    }
    if (a<INT_MIN/b){
        printf("Error: numeric overflow in MUL.\n");
        exit(1);
    }
    return res=a*b;
}
/*Gestisce solamente il caso di divisione per 0.*/
int safe_div(int a, int b){
    int res=0;
    if (b!=0){
        return res=a/b;
    }
    else{
        printf("Error: attempted division by 0.\n");
        exit(1);
    }
}

/*La funzione 'execute' prende come input l'array di interi restituita dalla funzione 'machine_code',
inizializza lo stack ed i registri, pone SP e IP a 0 ed esegue il ciclo fetch->decode->execute finché
non arriva all'ultima istruzione dell'array che è HALT. Inoltre gestisce eventuali errori come ad esempio
l'accesso a un registro fuori da quelli predisposti per l'utilizzo (>=0 e <32) o la lettura di un'istruzione sconosciuta.*/
void execute(int *program, int instruction_count){
    unsigned int SP = 0;/*Dichiaro i registri speciali IP e SP.*/
    unsigned int IP = 0;
    unsigned int stack_size=(unsigned int)STACK_SIZE;
    unsigned int supp=(unsigned int)instruction_count;
    bool running = true;/*quando diventa false (in HALT) l'esecuzione è giunta a termine*/
    bool is_jmp = false;/*diventa true solo quando il salto è preso, altrimenti in tutti gli altri casi viene posto a false*/
    int j, ris=0;
    int stack[STACK_SIZE];
    int registers[NUM_REGS];
    while (running && IP<supp){/*viene letta ogni istruzione dell'array fino a quando almeno una delle due condizioni diventa falsa*/
        if (program[IP]==HALT){/*il programma deve terminare quando raggiunge questa istruzione, infatti solo in essa running è posto a false*/
            running=false;
            printf ("Finished Execution.\n");
        }
        else if (program[IP]==DISPLAY){/*stampa su console il risultato calcolato dalla funzione*/
            check_valid_registers(program[IP+1], NUM_REGS);/*per ogni istruzione che utilizza i resitri controllo se tale registro è valido, cioè se è >=0 e <32*/
            printf ("R%d = %d\n", program[IP+1], registers[program[IP+1]]);
            IP++;
            is_jmp=false;
        }
        else if (program[IP]==PRINT_STACK){/*stampa in ordine inverso i valori contenuti nello stack con le loro rispettive posizioni a partire da SP-1 fino a SP-N inclusa*/
            printf("Number of stack positions: %d\n", program[IP+1]);
            if ((int)SP - program[IP+1] < 0){/*se il numero di elementi per i quali è richiesta la stampa è maggiore di quelli disponibili, vengono stampati tutti quelli presenti, e poi viene segnalato un underflow*/
                for (j=(int)(SP-1); j>=0; j--)
                    printf("SP: %d = %d\n", j, stack[j]);
                printf("STACK UNDERFLOW\n");
                exit(1);
            }
            else {
                for (j=(int)(SP-1); j>=(int)SP-program[IP+1]; j--)/*se invece questo numero non crea problemi, i valori vengono stampati regolarmente*/
                    printf("SP: %d = %d\n", j, stack[j]);
            }
            IP++;
            is_jmp=false;
        }
        else if (program[IP]==PUSH){/*se lo stack non è saturo ci vado a inserire il valore contenuto nel registro indicato*/
            check_valid_registers(program[IP+1], NUM_REGS);
            if (SP<stack_size){/*controllo prima se lo stack non è saturo, altrimenti termino il programma*/
                stack[SP]=registers[program[IP+1]];
                IP++;
                SP++;
                is_jmp=false;
            }
            else{
                printf("STACK OVERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==POP){/*se lo stack non è vuoto inserisco il valore che si trova in cima allo stack nel registro indicato*/
            check_valid_registers(program[IP+1], NUM_REGS);
            if (SP!=0){
                SP--;/*decremento SP prima di copiare il valore in cima allo stack nel registro indicato*/
                registers[program[IP+1]]=stack[SP];
                IP++;
                is_jmp=false;
            }
            else{
                printf("STACK UNDERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==MOV){/*copia il valore del secondo parametro (numero intero) nel registro indicato*/
            check_valid_registers(program[IP+1], NUM_REGS);
            registers[program[IP+1]]=program[IP+2];
            IP=IP+2;
            is_jmp=false;
        }
        else if (program[IP]==CALL){
            if (SP<stack_size){
                IP++;
                stack[SP]=IP+1;/*salvo l'indice dell'istruzione successiva a call*/
                SP++;/*incremento SP in quanto ho fatto una push di IP*/
                IP=program[IP];/*salto alla posizione indicata*/
                is_jmp=true;/*IP non incrementa più alla fine del ciclo while in quanto il salto è stato preso*/
            }
            else{
                printf("STACK OVERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==RET){
            if (SP!=0){
                SP--;
                IP=stack[SP];/*viene recuperata l'istruzione che avevo salvato dentro lo stack per tornare dalla chiamata alla subroutine*/
                is_jmp=true;
            }
            else{
                printf("STACK UNDERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==JMP){/*si salta incondizionatamente all'istruzione indicata*/
            IP=program[IP+1];
            is_jmp=true;
        }
        else if (program[IP]==JZ){/*si salta all'istruzione indicata solo se il valore in cima allo stack è uguale a 0*/
            if (SP!=0){/*verifico prima se lo stack non è vuoto*/
                SP--;
                if (stack[SP]==0){
                    IP=program[IP+1];
                    is_jmp=true;/*il salto è preso se la condizone si verifica*/
                }
                else{
                    is_jmp=false;
                    IP++;
                }
            }
            else{
                printf("STACK UNDERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==JPOS){/*si salta all'istruzione indicata solo se il valore in cima allo stack è maggiore di 0*/
            if (SP!=0){/*verifico prima se lo stack non è vuoto*/
                SP--;
                if (stack[SP]>0){
                    IP=program[IP+1];
                    is_jmp=true;/*il salto è preso se la condizone si verifica*/
                }
                else{
                    is_jmp=false;
                    IP++;
                }
            }
            else{
                printf("STACK UNDERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==JNEG){/*si salta all'istruzione indicata solo se il valore in cima allo stack è minore di 0*/
            if (SP!=0){/*verifico prima se lo stack non è vuoto*/
                SP--;
                if (stack[SP]<0){
                    IP=program[IP+1];
                    is_jmp=true;/*il salto è preso se la condizone si verifica*/
                }
                else{
                    is_jmp=false;
                    IP++;
                }
            }
            else{
                printf("STACK UNDERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==ADD){
            check_valid_registers(program[IP+1], NUM_REGS);/*controllo che i registri indicati siano validi, cioè >=0 e <32*/
            check_valid_registers(program[IP+2], NUM_REGS);
            ris=safe_add(registers[program[IP+1]], registers[program[IP+2]]);/*chiamo la funzione che mi calcola la somma in modo sicuro, cioè gestendo il caso di overflow aritmetico*/
            IP=IP+2;
            if (SP<stack_size){/*controllo prima che lo stack non sia saturo*/
                stack[SP]=ris;
                SP++;
                is_jmp=false;
            }
            else{
                printf("STACK OVERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==SUB){
            check_valid_registers(program[IP+1], NUM_REGS);
            check_valid_registers(program[IP+2], NUM_REGS);
            ris=safe_sub(registers[program[IP+1]], registers[program[IP+2]]);/*chiamo la funzione che mi calcola la sottrazione in modo sicuro*/
            IP=IP+2;
            if (SP<stack_size){/*controllo prima che lo stack non sia saturo*/
                stack[SP]=ris;
                SP++;
                is_jmp=false;
            }
            else{
                printf("STACK OVERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==MUL){
            check_valid_registers(program[IP+1], NUM_REGS);
            check_valid_registers(program[IP+2], NUM_REGS);
            ris=safe_mul(registers[program[IP+1]], registers[program[IP+2]]);/*chiamo la funzione che mi calcola la moltiplicazione in modo sicuro*/
            IP=IP+2;
            if (SP<stack_size){/*controllo prima che lo stack non sia saturo*/
                stack[SP]=ris;
                SP++;
                is_jmp=false;
            }
            else{
                printf("STACK OVERFLOW\n");
                exit(1);
            }
        }
        else if (program[IP]==DIV){
            check_valid_registers(program[IP+1], NUM_REGS);
            check_valid_registers(program[IP+2], NUM_REGS);
            ris=safe_div(registers[program[IP+1]], registers[program[IP+2]]);/*chiamo la funzione che mi calcola la divisione in modo sicuro*/
            IP=IP+2;
            if (SP<stack_size){/*controllo prima che lo stack non sia saturo*/
                stack[SP]=ris;
                SP++;
                is_jmp=false;
            }
            else{
                printf("STACK OVERFLOW\n");
                exit(1);
            }
        }
        else{
            printf("UNKNOWN INSTRUCTION: %d\n", program[IP]);/*se il programma incontra un'istruzione che non è definita nell'instruction_set, l'errore viene stampato su console e si esce dal programma*/
            exit(1);
        }
        if (!is_jmp){/*se non si tratta di un'istruzione di salto condizionato o incondizionato, IP viene incrementato in modo da puntare alla prossima istruzione*/
            IP++;
        }
    }
}
