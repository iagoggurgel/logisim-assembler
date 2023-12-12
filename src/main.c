// Author - Iago Gouveia Gurgel
// Date - 12/04/2023


// Definition of macros

#define LINESIZE 100UL
#define INT16MAX 65535UL
#define INT32MAX 4294967295UL

#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// print functions
void printHelp();
void printAuthor();

// simulate functions

// verbose functions

// function signatures for assembler
int main(int, char **);
void checkForOpt(int, char **);
int run(int, char **);
char ** parseLine(char *, cJSON *);
char * getInst(char *);
int findWhitespaceIndex(char *);
char * getReg(int, char *);
char * getImmd(char *);
char * getLabel(char *);
int assemble(cJSON * , char **, cJSON *, cJSON *);
int saveResult(int *, char *);

// function signatures for configuration
char * getContent(FILE *);
unsigned long getSize(FILE *);
cJSON * setup(char *);

cJSON * setup(char * filename){

    FILE * fp;

    fp = fopen(filename, "r");

    if (fp == NULL){
        printf(".set file does not exist.\n");
        exit(EXIT_FAILURE);
    }

    char * fileContent = getContent(fp);

    fclose(fp);

    cJSON *root = cJSON_Parse(fileContent);

    if (root == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    return root;
}

char * getContent(FILE * fp){

    unsigned long fileSize = getSize(fp);

    rewind(fp);

    char * fileContent = (char *) malloc(sizeof(char) * (fileSize + 1));

    for (int i = 0; i < fileSize; i++){
        fileContent[i] = fgetc(fp);
    }

    fileContent[fileSize] = '\0';

    return fileContent;

}

unsigned long getSize(FILE * fp){

    fseek(fp, 0L, SEEK_END);

    return ftell(fp);

}

void checkForOpt(int argc,char ** argv){

    for(int i = 0; i < argc; i++){
        if (!strcmp(argv[i], "--h") || !strcmp(argv[i], "-h")){
            printHelp();
            exit(EXIT_SUCCESS);
        }

        else if (!strcmp(argv[i], "--a") || !strcmp(argv[i], "-a")){
            printAuthor();
            exit(EXIT_SUCCESS);
        }

        else {
            if(run(argc, argv) == 0){
                printf("Assembly coded!\n");
                exit(EXIT_SUCCESS);
            }
        }
    }
}

void printHelp(){
    system("cls || clear");
    printf("Uso: ./assemble [ARQUIVO].set [ARQUIVO].asm [OUTPUT] [OPÇÃO] ...\n");
    printf("Realiza a montagem do assembly no código de máquina da arquitetura descrita no .set e produz um arquivo de memória logisim\n");
    printf("\nOPÇÕES\n");
    printf("-h, --h         mostra a ajuda e não executa.\n");
    printf("-a, --a         mostra as informações sobre o autor e não executa.\n");
    printf("-sim, --sim         executa a montagem linha a linha e imprime uma simulação da instrução, registradores, memória de dados e Program Counter.\n");
    printf("-verbose, --verbose         executa e imprime o passo a passo da montagem.\n");
}

void printAuthor(){
    system("cls || clear");
    printf("------------------------------ \n");
    printf("Nome do Autor: Iago Gouveia Gurgel\n");
    printf("Período: 2023.2\n");
    printf("Disciplina: Programação\n");
    printf("------------------------------ \n");
}

char ** parseLine(char * line, cJSON * root){

    char * asmInst = getInst(line);

    cJSON *argsArray = cJSON_GetObjectItem(root, "instructions");

    argsArray = cJSON_GetObjectItem(argsArray, asmInst);

    argsArray = cJSON_GetObjectItem(argsArray, "args");

    char ** parsedLine = (char **) malloc(sizeof(char *) * (cJSON_GetArraySize(argsArray) + 1));

    parsedLine[0] = asmInst;

    for (int i = 0; i < cJSON_GetArraySize(argsArray); i++){
        char * substring = (char *) malloc(sizeof(char) * LINESIZE);
        if (!strcmp("reg", cJSON_GetArrayItem(argsArray, i)->valuestring))
        {
            substring = getReg(i, line);
            parsedLine[i + 1] = substring;
            continue;
        }

        else if (!strcmp("immd", cJSON_GetArrayItem(argsArray, i)->valuestring))
        {
            substring = getImmd(line);
            parsedLine[i + 1] = substring;
            continue;
        }

        else if (!strcmp("label", cJSON_GetArrayItem(argsArray, i)->valuestring))
        {
            substring = getLabel(line);
            parsedLine[i + 1] = substring;
            continue;
        }

        parsedLine[i + 1] = NULL;
    }

    return parsedLine;


}

char * getLabel(char * line){
    char * label = (char *) malloc(sizeof(char) * 20);
    memset(label, 0, 20);
    int index = 0;

    char * duplicate = line;

    for (int i = 0; i < strlen(duplicate) - 1; i++){
        if (duplicate[i] == '_'){
            index = i + 1;
            break;
        }
    }

    if (index == 0){
        printf("No match found!");
        exit(EXIT_FAILURE);
    }

    duplicate = duplicate + index;

    int j = 0;

    while (!(*duplicate == '\0'))
    {
        label[j] = *duplicate;
        duplicate += 1;
        j += 1;
    }
    
    return label;

}

char * getImmd(char * line){
    char * immd = (char *) malloc(sizeof(char) * 20);
    memset(immd, 0, 20);
    int index = 0;

    char * duplicate = line;
    
    for (int i = 0; i < strlen(duplicate) - 1; i++){
        if (((duplicate[i] << 8) + duplicate[i + 1]) == '0x'){
            index = i + 2;
            break;
        }
    }

    if (index == 0){
        printf("No match found!");
        exit(EXIT_FAILURE);
    }

    duplicate = duplicate + index;

    int j = 0;

    while (!(*duplicate == '\0'))
    {
        immd[j] = *duplicate;
        duplicate += 1;
        j += 1;
    }
    
    return immd;
}

char * getReg(int counter, char * line){

    char * regName = (char *) malloc(sizeof(char) * 20);
    memset(regName, 0, 20);
    int index = 0;

    char * duplicate = line;
    int internal = 0;

    for (int i = 0; i < strlen(duplicate); i++){
        if (duplicate[i] == '$'){
            
            if (internal == counter){
                index = i;
                break;
            }
            internal += 1;
        }
    }

    if (index == 0){
        printf("No match found!");
        exit(EXIT_FAILURE);
    }

    index = index + 1;
    duplicate = duplicate + index;

    int j = 0;

    while (!(*duplicate - '\0' == 44 || *duplicate - '\0' == 32 || *duplicate == '\0'))
    {
        regName[j] = *duplicate;
        duplicate += 1;
        j += 1;
    }
    
    return regName;

}

char * getInst(char * line){

    int whitespaceIndex = findWhitespaceIndex(line);

    char * instruction = (char *) malloc(sizeof(char) * (whitespaceIndex + 1));

    for (int i = 0; i < whitespaceIndex; i++){
        instruction[i] = line[i];
    }

    instruction[whitespaceIndex] = '\0';
 
    return instruction;
}

int findWhitespaceIndex(char * line){

    for (int i = 0; i < strlen(line); i++){
        if (line[i] - '\0' == 32){
            return i;
        }
    }
}

int main(int argc, char ** argv){

    checkForOpt(argc, argv);

    return 0;
}

int run(int argc, char ** argv){

    cJSON * root = setup(argv[1]);

    cJSON * types = cJSON_GetObjectItem(root, "types");

    cJSON * rBank = cJSON_GetObjectItem(root, "registerBank");

    rBank = cJSON_GetObjectItem(rBank, "regBank");


    cJSON * instructions = cJSON_GetObjectItem(root, "instructions");

    FILE * asmFile = fopen(argv[2], "r");

    if (asmFile == NULL){
        printf("asm file does not exist\n");
        exit(EXIT_FAILURE);
    }
    
    int * memory = (int *) malloc(sizeof(int) * (INT16MAX + 1));

    int counter = 0;

    char * line;

    char ** parsedLine;

    cJSON * instruction;

    int instructionI;

    while(!feof(asmFile)){
        
        line = malloc(sizeof(char) * LINESIZE);

        fgets(line, LINESIZE, asmFile);

        parsedLine = parseLine(line, root);

        instruction = cJSON_GetObjectItem(instructions, parsedLine[0]);

        instructionI = assemble(instruction, parsedLine, types, rBank);

        memory[counter] = instructionI;

    }

    saveResult(memory, argv[3]);

    return 0;
}

int assemble(cJSON * inst, char ** parseLine, cJSON * types, cJSON * regBank){

    int instruction = 0;

    cJSON * instType = cJSON_GetObjectItem(types, cJSON_GetObjectItem(inst, "type")->valuestring);

    int * fields = (int *) malloc(sizeof(int) * cJSON_GetArraySize(instType));

    int * fieldSizes = (int *) malloc(sizeof(int) * cJSON_GetArraySize(instType));

    cJSON * typeField;

    cJSON * fieldId;

    cJSON * fieldSize;

    cJSON * field;

    int temp;

    for(int i = 0; i < cJSON_GetArraySize(instType); i++){

        typeField = cJSON_GetArrayItem(instType, i);

        fieldId = cJSON_GetObjectItem(typeField, "id");

        fieldSize = cJSON_GetObjectItem(typeField, "size");

        field = cJSON_GetObjectItem(inst, "fields");

        temp = cJSON_GetObjectItem(field, fieldId->valuestring)->valueint;

        if (!strcmp(fieldId->valuestring, "rs") || !strcmp(fieldId->valuestring, "rt") || !strcmp(fieldId->valuestring, "rd")){

            fields[i] = cJSON_GetObjectItem(regBank, "zero")->valueint;
        }

        else
        {
            fields[i] = temp;
        }

        fieldSizes[i] = fieldSize->valueint;
    }

    int shift = 0;

    for(int i = 0; i < cJSON_GetArraySize(instType); i++){
        shift = shift + fieldSizes[i];

        instruction += fields[i] << (32 - shift);
    }

    printf("0x%08x\n", instruction);

    free(instType);
    free(fields);
    free(fieldSizes);
    free(typeField);
    free(fieldId);
    free(fieldSize);
    free(field);

    return instruction;
}

int saveResult(int * memory, char * filename){

    FILE * filePointer;

    filePointer = fopen(filename, "a");

    if(filePointer == NULL){
        return 1;
    }

    for(unsigned int counter = 0; counter < (INT16MAX + 1); counter++){
        if(counter % 8 == 0){
            fprintf(filePointer, "\n%04x : %08x", counter, memory[counter]);
            continue;
        }

        fprintf(filePointer, " %08x", memory[counter]);
    }
    
    return 0;

}