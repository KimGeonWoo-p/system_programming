#include "kvs.h"

//to recovery
int recover(kvs_t* kvs, int f)
{
    	char* buffer;

	if (!(buffer=(char*)malloc(sizeof(char)*BUFF_SIZE)))
	{
	    	close(f);
	    	return -1;
	}

	int n_read;

	while ((n_read = read(f, buffer, BUFF_SIZE)) > 0)
	{
	    	if (n_read < 0)
		{
			close(f);
			return -1;
	    	}

	    	off_t offset = 0;
	    	char line[10000];
	    	int b_idx = 0;
	    	int l_idx = 0;
	    	int last = 0;
	    	char cur;

	    	while (b_idx < n_read)
	    	{
			cur = buffer[b_idx++];
			if (cur == '\n')
			{
		    		line[l_idx] = '\0';

		    		char* key = strtok(line, ",");
		    		char* value = strtok(NULL, ",");

		    		set(kvs, key, value);

				last = b_idx-1;
				l_idx = 0;
				continue;
			}
			line[l_idx++] = cur;
		}

		if (n_read < BUFF_SIZE)
			break;

		offset = last - BUFF_SIZE + 1;
		lseek(f, offset, SEEK_CUR);
	}

	close(f);
	printf("recovery soccess!! %d data\n", kvs->items);
	return kvs->items;
}
