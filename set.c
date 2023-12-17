#include "kvs.h"

//set
int set(kvs_t* kvs, char* key, char* value)
{
    node_t* new_node;

    //find element with such key
    HASH_FIND_STR(kvs->db_hash, key, new_node);
    char* new_key;
    char* new_value;

    if (!new_node)
    {
        //if failing to allocate new_node
        if (!(new_node = (node_t*)malloc(sizeof(node_t))))
            return -1;

        size_t k_size = strlen(key);
        size_t v_size = strlen(value);
        new_node->k_size = k_size;
        new_node->v_size = v_size;

	new_key = (char*)malloc(sizeof(char)*(k_size+1));
	new_value = (char*)malloc(sizeof(char)*(v_size+1));
	
	strcpy(new_key, key);
        strcat(new_key, "\0");
        strcpy(new_value, value);
        strcat(new_value, "\0");
        
        new_node->key = new_key;
        new_node->value = new_value;
        HASH_ADD_STR(kvs->db_hash, key, new_node);
        kvs->items++;
        return v_size;
    }

    //set value of element with such key
    free(new_node->value);
    
    size_t v_size = strlen(value);
    new_node->v_size = v_size;
    new_value = (char*)malloc(sizeof(char)*(v_size+1));
    
    strcpy(new_value, value);
    strcat(new_value, "\0");
    new_node->value = new_value;
    return v_size;
}
