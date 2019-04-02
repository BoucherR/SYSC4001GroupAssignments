all:	server client

CC=cc
CFLAGS= -pedantic -Wall

# For debugging un-comment the next line
# DFLAGS=-DDEBUG_TRACE=1 -g


DBM_INC_PATH=/usr/include/gdbm
DBM_LIB_FILE=-lgdbm
# On some distributions you may need to change the above line to include
# the compatibility library, as shown below.
# DBM_LIB_FILE=-lgdbm_compat -lgdbm



.c.o:
	$(CC) $(CFLAGS) -I$(DBM_INC_PATH) $(DFLAGS) -c $<

app_ui.o: app_ui.c cd_data.h
cd_dbm.o: cd_dbm.c cd_data.h
client_f.o: clientif.c cd_data.h cliserv.h
ipc_imp.o: ipc_imp.c cd_data.h cliserv.h
server.o: server.c cd_data.h cliserv.h


client: app_ui.o clientif.o ipc_imp.o
	$(CC) -o client  $(DFLAGS) app_ui.o clientif.o ipc_imp.o

server:	server.o cd_dbm.o ipc_imp.o
	$(CC) -o server $(DFLAGS) server.o cd_dbm.o ipc_imp.o $(DBM_LIB_FILE)

clean:
	rm -f server client *.o *~

