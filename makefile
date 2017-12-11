C=gcc
CFLAGS=
LDFLAGS=
EXEC=file_dissector modify_gzip

all: $(EXEC)

file_dissector: file_dissector.c
		$(CC) -o $@ $^ $(LDFLAGS)

modify_gzip: 	modify_gzip.c
		$(CC) -o $@ $^ $(LDFLAGS)

clean:
		rm -rf *.o
