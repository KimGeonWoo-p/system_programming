#include "kvs.h"

//open kvs.
//if in_memory_data_set is already prepared,
//open it and recover data structure in opened kvs.
kvs_t* kvs_open()
{
    kvs_t* kvs;
    int f;
    char* backup = "kvs.img";
    
    if(!(kvs = (kvs_t*) malloc (sizeof(kvs_t))))
        return NULL;

    if(kvs)
        kvs->items = 0;

    //if backup file is exist,
    //do recovery.
    
    int result = 0;
    if ((f=open(backup,O_RDONLY))>0)
    {
    	if((result=recover(kvs, f)) < 0)
    	{
    	    printf("recovery failed!!\n");
    	    close(f);
    	    return NULL;
    	}
    }
    printf("%d items are recovered!!\n", result);
    close(f);
    return kvs;
}

