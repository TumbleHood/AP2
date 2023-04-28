.PHONY: all
all: coder

coder: coder.c coder.h codec.h
	gcc coder.c ./libCodec.so -o coder -pthread

.PHONY: clean
clean:
	-rm coder 2>/dev/null
