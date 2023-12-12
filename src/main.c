// Author - Iago Gouveia Gurgel
// Date - 12/04/2023


// Definition of macros

#define LINESIZE 25UL
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
int run(int argc, char ** argv);
char ** parseLine(char *, cJSON *);
char * getInst(char *);
int findWhitespaceIndex(char *);
char * getReg(int, char *);
char * getImmd(char *);
char * getLabel(char *);

// function signatures for configuration
char * getContent(FILE *);
unsigned long getSize(FILE *);
cJSON * setup(char * filename);

cJSON * setup(char * filename){

    FILE * fp;

    fp = fopen(filename, "r");

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
        }

        else if (!strcmp(argv[i], "--a") || !strcmp(argv[i], "-a")){
            printAuthor();
        }

        else {
            run(argc, argv);
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

    memset(parsedLine, 0, sizeof(char *) * (cJSON_GetArraySize(argsArray) + 1));

    parsedLine[0] = asmInst;

    for (int i = 1; i < cJSON_GetArraySize(argsArray) + 1; i++){
        char * substring = (char *) malloc(sizeof(char) * LINESIZE);
        if (!strcmp("reg", cJSON_GetArrayItem(argsArray, i)->valuestring))
        {
            substring = getReg(i, line);
            parsedLine[i] = substring;
        }

        else if (!strcmp("immd", cJSON_GetArrayItem(argsArray, i)->valuestring))
        {
            substring = getImmd(line);
            parsedLine[i] = substring;
        }

        else if (!strcmp("label", cJSON_GetArrayItem(argsArray, i)->valuestring))
        {
            substring = getLabel(line);
            parsedLine[i] = substring;
        }
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
        exit(1);
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
        exit(1);
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
        exit(1);
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

    cJSON * root = setup("instSets/mips.set");

    char ** parsedLine = parseLine("addi $t0, $t1, 0x0000", root);

    printf("%s", parsedLine[0]);

    exit(0);

    checkForOpt(argc, argv);

    return 0;
}

int run(int argc, char ** argv){
    return 0;
}