.PHONY: all
all: task stdinExample

task:	codec.h basic_main.c
	gcc basic_main.c -L. -l Codec -o encoder

stdinExample:	codec.h stdin_main.c
		gcc stdin_main.c -L. -l Codec -o tester

main: main.c main.h codec.h
	gcc main.c -L. -l Codec -o main -pthread -Wl,-rpath=/root/projects/AP2/

.PHONY: clean
clean:
	-rm encoder tester 2>/dev/null
