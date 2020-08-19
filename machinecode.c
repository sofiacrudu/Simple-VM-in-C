/*Matricola = 876335, Nome Gruppo = P1G66*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "vm.h"

/*La seguente funzione serve a estrapolare i singoli numeri corrispondenti al codice macchina;
dunque, con l'appoggio della funzione di libreria 'isdigit', prende soltanto i numeri all'inizio
di ogni riga seguiti dal carattere ';' oppure '\n'.Inoltre, la funzione ha un side-effect
sulla stringa data in input, ripulendola di eventuali commenti, così da poter essere utilizzata dopo
nella funzione 'machine_code' con la sscanf.*/
int capture_numbers(char *str){
    int i=0, j=0, check=1;
    while (str[i]!='\0' && str[i]!=';' && str[i]!='\n' && str[i]!='\r' && check){/*leggo la stringa fino a quando non raggiungo uno dei caratteri dati*/
        if (isdigit(str[i]) || str[i]==32 || (str[i]=='-' && isdigit(str[i+1]))){/*devo leggere solo numeri interi e spazi*/
            str[j]=str[i];
            j++;
        }
        else
            check=0;
        i++;
    }
    str[j]='\0';
    return check;/*restituisce 1 se la stringa letta contiene un numero, 0 altrimenti*/
}

/*Questa funzione serve a ignorare le linee vuote o commentate.*/
int ignore_lines(char *str){
    int i=0, check=1;
    while (str[i]!='\0' && str[i]!=';' && str[i]!='\n' && str[i]!='\r'){/*scorre la stringa finché non trova uno dei caratteri indicati*/
        if (str[i]!=' ')
            check=0;
        i++;
    }
    return check;/*restituisce 1 se la linea è vuota o contiene commenti, 0 altrimenti*/
}
/*La funzione 'machine_code' riceve in input un file con estensione .cvm e restituisce un array di interi contenente
i singoli numeri corrispondenti al codice macchina; inoltre la funzione memorizza la dimensione dell'array
in un puntatore a int (instruction_count) che verrà utilizzata successivamente nelle funzioni 'execute' e 'print'.
Per eventuali errori di apertura del file, o ricezione di input sbagliati, il programma viene interrotto subito,
stampando l'errore che ne ha provocato la chiusura.*/
int* machine_code(char *inFileName, int *instruction_count){
    unsigned int IP = 0;
    unsigned int supp;/*serve ad accogliere la dimensione dell'instruction count per confrontarlo con IP che è di tipo unsigned int*/
    int *array_int=NULL;
    char *buff=NULL;
	size_t nbytes=0;
	ssize_t line;
    FILE *file;
    int is_num, stop=0;
    long int n;/*serve a memorizzare con sscanf il numero letto e confrontarlo con INT_MAX o INT_MIN per vedere se c'è overflow nel numero*/
    int instruction_number;
    file=fopen(inFileName, "r");
    if (file==NULL){
        printf("Error: could not open the file.\n");
        exit(1);
    }
    else {
        line=getline(&buff, &nbytes, file);
        while (line!=-1 && !stop){/*legge il file con getline e quando stop==1 vuol dire che ha trovato il numero per la dimensione dell'array*/
            if (!ignore_lines(buff)){/*ignoro le linee vuote o commentate*/
                is_num=capture_numbers(buff);
                sscanf(buff, "%ld", &n);/*leggo il numero da stringa per confrontarlo con INT_MAX e INT_MIN in modo da vedere se si è verificato overflow*/
                if (is_num){
                    if (n>INT_MAX || n<INT_MIN){
                        printf("INTEGER OVERFLOW = %ld\n", n);/*segnalo l'overflow per il numero intero*/
                        free(buff);
                        exit(1);
                    }
                }
                *instruction_count=atoi(buff);/*se non si è verificato overflow, converto il numero in un intero*/
                if (is_num && (*instruction_count>=0)){/*il numero per la dimensione dell'array deve essere necessariamente valido e positivo*/
                    stop=1;/*esco dal ciclo while*/
                }
                else {
                    printf("Error: no valid input for instruction count.\n");
                    free(buff);
                    exit(1);
                }
            }
            if (stop){/*alloco l'array con la sua dimensione*/
                array_int=(int*)malloc((*instruction_count)*sizeof(int));
            }
            else {
                line=getline(&buff, &nbytes, file);/*se stop è ancora 0 vado avanti a leggere le righe del file*/
            }
        }
        if (array_int==NULL){/*controllo se la malloc è andata a buon fine altrimenti libero il buffer che ha accolto la stringa e ritorno NULL*/
            free(buff);
            return NULL;
        }
        else{
            supp=(unsigned int)(*instruction_count);/*mi serve una variabile di tipo unsigned int per fare il confronto con il registro speciale IP*/
            line=getline(&buff, &nbytes, file);
            while (line!=-1){/*eseguo lo stesso procedimento di prima fino a quando non finiscono le righe del file*/
                if (!ignore_lines(buff)){
                    is_num=capture_numbers(buff);
                    sscanf(buff, "%ld", &n);
                    if (is_num){
                        if (n>INT_MAX || n<INT_MIN){
                            printf("INTEGER OVERFLOW = %ld\n", n);/*segnalo l'overflow per il numero intero*/
                            free(array_int);
                            free(buff);
                            exit(1);
                        }
                    }
                    instruction_number=atoi(buff);
                    if (is_num && IP<supp){/*se si tratta di un numero e il suo indice è valido rispetto alla dimensione dell'array, va inserito*/
                        array_int[IP]=instruction_number;
                        IP++;
                    }
                    else if (IP>=supp){
                        printf("Error: array index out of bounds.\n");/*il numero predisposto per la dimensione dell'array non è sufficiente per accogliere altri valori*/
                        free(array_int);
                        free(buff);
                        exit(1);
                    }
                    else {
                        printf("Error: no valid input for machine code.\n");/*se viene letto qualcosa che non è nè un'istruzione nè un numero*/
                        free(array_int);
                        free(buff);
                        exit(1);
                    }
                }
                line=getline(&buff, &nbytes, file);
            }
        }
    }
    free(buff);
    fclose(file);
    return array_int;
}
