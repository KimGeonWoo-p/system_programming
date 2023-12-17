#include "kvs.h"

//get
char* get(kvs_t* kvs, char* key)
{
    node_t* result;
    HASH_FIND_STR(kvs->db_hash, key, result);
    
    if (!result)
        return NULL;

    return result->value;
}
