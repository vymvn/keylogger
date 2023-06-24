CC = gcc
WIN_CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -g
WIN_CFLAGS = -lws2_32 -mwindows -Os -s -fPIC


all: server client

server:
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/server src/server.c

client:
	mkdir -p bin
	$(WIN_CC) -o bin/win_client.exe src/win_client.c $(WIN_CFLAGS)

clean:
	rm -rf ./bin/* 
