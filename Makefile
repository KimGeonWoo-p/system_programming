SRCS = server.c client.c
LIBSRCS = kvs_open.c kvs_close.c set.c get.c capture.c recover.c 
TARGS = kvs_server kvs_client

CC = gcc

OBJS = $(SRCS:.c=.o)
LOBJS = $(LIBSRCS:.c=.o)
LARS = libkvs.a

LD_LIBRARY_PATH = libkvs.so

all: kvs_server kvs_client

kvs_server: server.o $(LD_LIBRARY_PATH)
	$(CC) -static server.o -pg -o kvs_server -ldl

kvs_client: client.o  $(LD_LIBRARY_PATH)
	$(CC) -static client.o -pg -o kvs_client -ldl

$(LD_LIBRARY_PATH): $(LOBJS)
	$(CC) -shared -o $(LD_LIBRARY_PATH) $(LOBJS) -lm

%.o: %.c
	$(CC) -fPIC -c $< -o $@

server.o: server.c
	$(CC) -c server.c -o server.o

client.o: client.c
	$(CC) -c client.c -o client.o

clean:
	rm -f $(TARGS) $(OBJS) $(LOBJS) $(LARS) $(LD_LIBRARY_PATH)



