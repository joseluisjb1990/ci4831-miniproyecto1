CC=gcc
CFLAGS=-c -Wall -std=c99

all: scdax_cli scdax_svr

scdax_cli: die_with_error.o scdax_cli.o
	$(CC) die_with_error.o scdax_cli.o -o scdax_cli

scdax_svr: die_with_error.o scdax_svr.o handle_client.o
	$(CC) die_with_error.o scdax_svr.o handle_client.o -o scdax_svr

scdax_cli.o: scdax_cli.c
	$(CC) $(CFLAGS) scdax_cli.c

die_with_error.o: die_with_error.c
	$(CC) $(CFLAGS) die_with_error.c

handle_client.o: handle_client.c
	$(CC) $(CFLAGS) handle_client.c

clean:
	rm *o scdax_cli scdax_svr

