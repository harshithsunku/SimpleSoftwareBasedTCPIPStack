CC=gcc
CFLAGS=-g
TARGET:main

INCLUDES=-I GenericEmbeddableDoublyLinkedList/ -I ThreadSafeCLogger/ -I infra/ -I common/

OBJS=GenericEmbeddableDoublyLinkedList/dll.o \
		  ThreadSafeCLogger/logger.o 		 \
		  infra/network_graph.o              \
		  common/topologies.o  

main:main.o ${OBJS}
	${CC} ${CFLAGS} main.o ${OBJS} -o main -lpthread

main.o:main.c
	${CC} ${CFLAGS} ${INCLUDES} -c main.c -o main.o

infra/network_graph.o:infra/network_graph.c
	${CC} ${CFLAGS} ${INCLUDES} -c infra/network_graph.c -o infra/network_graph.o

GenericEmbeddableDoublyLinkedList/dll.o:GenericEmbeddableDoublyLinkedList/dll.c
	${CC} ${CFLAGS} ${INCLUDES} -c GenericEmbeddableDoublyLinkedList/dll.c -o GenericEmbeddableDoublyLinkedList/dll.o

ThreadSafeCLogger/logger.o:ThreadSafeCLogger/logger.c
	${CC} ${CFLAGS} ${INCLUDES} -c ThreadSafeCLogger/logger.c -o ThreadSafeCLogger/logger.o

common/topologies.o:common/topologies.c
	${CC} ${CFLAGS} ${INCLUDES} -c common/topologies.c -o common/topologies.o

clean:
	rm *.o
	rm GenericEmbeddableDoublyLinkedList/dll.o
	rm ThreadSafeCLogger/logger.o
	rm infra/network_graph.o
	rm common/topologies.o
	rm main

