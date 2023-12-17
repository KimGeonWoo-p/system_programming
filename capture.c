#include "kvs.h"

//store data structure
//first, fill buffer with format (key, value).
//the unit of (key, value) data is stored completely on buffer. ex) buffer = "key1,value1\nkey2,value2". not "key1,val"
//when size of written data is less or equal but almost same with BUFF_SIZE, stop filling.
//second, write buffer on kvs.img
int capture(kvs_t* kvs, char* name)
{
    //for hash_itter
    node_t *current, *tmp;
    //for "kvs.img"
    int f;
    //for buffer
    char* buffer;
    //to chack the size of read data
    size_t nwrite;

    //if fail to open "kvs.img"
    if((f = open(name, O_CREAT | O_WRONLY, 0777)) < 0)
        return -1;

    //if fail to allocate buffer
    if (!(buffer = (char*) malloc(sizeof(char) * BUFF_SIZE)))
        return -1;

    char* cur_b_pos = buffer;		//for pointer position of buffer
    char* cur_k_pos;			//for pointer position of key
    char* cur_v_pos; 			//for pointer position of value
    int length = 0;

    HASH_ITER(hh, kvs->db_hash, current, tmp)
    {
        if(length + current->k_size + current->v_size + 2 > BUFF_SIZE)
        {
            if((nwrite=write(f, buffer, length)) < 0)
                return -1;
            cur_b_pos = buffer;
            length = 0;
        }
        cur_k_pos = current->key;
        cur_v_pos = current->value;

        while(*cur_k_pos != '\0')	//copy key
	    *cur_b_pos++ = *cur_k_pos++;
	
        *cur_b_pos++ = ',';
	
        while(*cur_v_pos != '\0')	//copy value
	    *cur_b_pos++ = *cur_v_pos++;
	
        *cur_b_pos++ = '\n';
        
        length += current->k_size + current->v_size + 2;
    }

    if (cur_b_pos > buffer)
    {
        *(cur_b_pos - 1) = '\n';
        nwrite = write(f, buffer, length);
    }
    
    free(buffer);
    close(f);
    return 0;
}
