/*Matricola = 876335, Nome Gruppo = P1G66*/
#define STACK_SIZE 16384/*Dichiaro la dimensione dello stack, che in tale caso è di 64KB.*/
#define NUM_REGS 32/*Dichiaro il numero dei registri.*/

/*L'enumerazione di ogni istruzione facilita la lettura del codice.*/
typedef enum{
    HALT,
    DISPLAY,
    PRINT_STACK, NOP3, NOP4, NOP5, NOP6, NOP7, NOP8, NOP9,/*le istruzioni che non ci sono nell'instruction_set vengono scritte solo per avere l'enumerazione corretta delle altre 16 istruzioni valide*/
    PUSH,
    POP,
    MOV, NOP13, NOP14, NOP15, NOP16, NOP17, NOP18, NOP19,
    CALL,
    RET,
    JMP,
    JZ,
    JPOS,
    JNEG, NOP26, NOP27, NOP28, NOP29,
    ADD,
    SUB,
    MUL,
    DIV
}INSTRUCTIONS;

int* machine_code(char *inFileName, int *instruction_count);

void execute(int *program, int instruction_count);

void print(int *program, int instruction_count);
