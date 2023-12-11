// Author - Iago Gouveia Gurgel
// Date - 12/04/2023


// Definition of macros

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MULTIPLIER 59
#define MAXCAPACITY 64
#define LINEMAXSIZE 1024


#define TK_TYPES_PATTERN "(types)|(=)|(\{[A-Za-z ,]*\})"

// Definition of structures

typedef struct
HashItem {
    
    char * key;
    void * data;


} HashItem;

typedef struct
HashTable {

    HashItem ** items;
    unsigned int size;
    unsigned int count;

} HashTable;


// Function signatures for hashtables

unsigned long hash_function(const char *, HashTable *);
HashTable * create_table();
void insert_item(HashTable *, const char *, void *);
HashItem * search_item(HashTable *, const char *);

// Functions for reading .set file

HashTable * setup_types_table(FILE *);
HashTable * setup_registers_bank(FILE *);
HashTable * setup_instructions_table(FILE *);


// Functions for Regex Utilization

regmatch_t * matchPattern(const char *, const char *, size_t);






int main(int, char **);
void * config_assembler(char **);


unsigned long hash_function(const char * key, HashTable * table){

    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) key;

    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 

    return h % table->size;
}

int main(int argc, char ** argv){
    return 0;
}

HashTable * create_table(){

    HashTable * newTable = (HashTable *) malloc(sizeof(HashTable));

    HashItem ** hashItems = (HashItem **) malloc(sizeof(HashItem *) * MAXCAPACITY);
    hashItems = (HashItem **) memset(hashItems, 0, sizeof(HashTable *) * MAXCAPACITY);

    newTable->items = hashItems;
    newTable->size = MAXCAPACITY;
    newTable->count = 0;

    return newTable;

}

void insert_item(HashTable * hashTable, const char * key, void * data){

    HashItem * newItem = (HashItem *) malloc(sizeof(HashItem));

    newItem->key = key;
    newItem->data = data;

    unsigned long index = hash_function(key, hashTable);

    hashTable->items[index] = newItem;

}

HashTable * setup_types_table(FILE * filePointer){

    HashTable * newTable = NULL;

    size_t expected_m = (size_t) 3;

    regmatch_t ** pmatch = (regmatch_t *) malloc(sizeof(regmatch_t *) * expected_m);
 
    while(!feof(filePointer)){

        char * buffer = (char *) malloc(sizeof(char) * LINEMAXSIZE);

        fgets(buffer, LINEMAXSIZE, filePointer);

        pmatch = matchPattern(TK_TYPES_PATTERN, buffer, expected_m);

        if(pmatch[0] == -1){
            continue;
        }

        

    }

}

regmatch_t * matchPattern(const char * pattern, const char * matchS, size_t expected_matches){

    regex_t * regex_pattern = (regex_t *) malloc(sizeof(regex_t));

    regmatch_t ** pmatch = (regmatch_t *) malloc(sizeof(regmatch_t *) * expected_matches);

    if (regcomp(regex_pattern, pattern, REG_EXTENDED | REG_ICASE) != 0) { printf("Error in REGEX Compilation"); exit(1); }

    if (regexec(regex_pattern, matchS, expected_matches, pmatch, 0) != 0) { printf("Error in REGEX Execute"); exit(1); } 

    regfree(regex_pattern);

    return pmatch;
}

