#include "kvs.h"

int kvs_close(kvs_t* kvs)
{
    node_t *current, *tmp;

    HASH_ITER(hh, kvs->db_hash, current, tmp) {
        free(current->key);
        free(current->value);
        HASH_DEL(kvs->db_hash, current);
        free(current);
    }
    free(kvs->db_hash);
    free(kvs);
    return 0;
}
