CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -pthread

SRCS = lab3.c
OBJS = $(SRCS:.c=.o)
EXEC = floyd_warshall

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

test: $(EXEC)
