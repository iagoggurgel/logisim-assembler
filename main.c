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
int assembleLine(int * fields, char * instType);

// Functions for reading .set file

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

int assembleLine(int * fields, char * instType){

    

}
