CC=gcc
TARGET=torcli.so
CFLAGS=-fPIC -shared -ldl -D_GNU_SOURCE

all:
	$(CC) torcli.c -o $(TARGET) $(CFLAGS)

clean:
	rm torcli.so

.PHONY:
	all
	clean

