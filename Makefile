CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -lncurses

all : start
	$(CC) $(CFLAGS) game_of_live.c -o ./build/game_of_live

start :
	mkdir -p ./build

clean :
	rm ./build/*
	rm -rf ./build 

format :
	clang-format -n *.c
	clang-format -i *.c