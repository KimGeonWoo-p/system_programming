#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>         // O_WRONLY
#include <unistd.h>        // write(), close()
#include <sys/types.h> 
#include "uthash.h"

#define BUFF_SIZE 100000

struct node {
	//allocate key and value on set function!!
	char* key;
	char* value;
	
	int k_size, v_size;
    UT_hash_handle hh;
};
typedef struct node node_t;

struct kvs{
	struct node* db_hash; // database
	int items; // number of data 
};
typedef struct kvs kvs_t; 

kvs_t* kvs_open();
int kvs_close(kvs_t* kvs); // free all memory space 
int set(kvs_t* kvs, char* key, char* value); // return -1 if failed.
char* get(kvs_t* kvs, char* key); // return NULL if not found.

//for snapshot
int capture(kvs_t* kvs, char* name);
int recover(kvs_t* kvs, int f);
