CC = gcc
CFLAGS = -Wall -D _CLID_SOURCE 
OBJETS = CL_msg.o CL_sem.o CL_data.o CL_mem.o Lecteur.o Moniteur.o Redacteur.o Driver.o

all: CL

CL : $(OBJETS) CL.c
	$(CC) $(CFLAGS) $^ -o CL

clean:
	@rm -f *~ *.o driver CL
