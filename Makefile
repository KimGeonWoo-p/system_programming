SRCS = server.c client.c
LIBSRCS = kvs_open.c kvs_close.c set.c get.c capture.c recover.c 
TARGS = kvs_server kvs_client

CC = gcc

OBJS = $(SRCS:.c=.o)
LOBJS = $(LIBSRCS:.c=.o)

LD_LIBRARY_PATH = libkvs.so

all: kvs_server kvs_client

kvs_server: server.o $(LD_LIBRARY_PATH)
	$(CC) server.o -pg -o kvs_server ./$(LD_LIBRARY_PATH)

kvs_client: client.o  $(LD_LIBRARY_PATH)
	$(CC) client.o -pg -o kvs_client ./$(LD_LIBRARY_PATH)

$(LD_LIBRARY_PATH): $(LIBSRCS)
	$(CC) -shared -fPIC $(LIBSRCS) -o $(LD_LIBRARY_PATH)

server.o: server.c
	$(CC) -c server.c -o server.o

client.o: client.c
	$(CC) -c client.c -o client.o

clean:
	rm -f $(TARGS) $(OBJS) $(LARS) $(LD_LIBRARY_PATH) $(LOBJS)



