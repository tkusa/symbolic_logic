CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

logic: $(OBJS)
	$(CC) -o logic $(OBJS) $(LDFLAGS) -lm

$(OBJS): logic.h

test: logic
	./test.sh

clean:
	rm -f logic *.o *~ tmp*

.PHONY: test clean