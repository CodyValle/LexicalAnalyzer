OUT = bin/LexicalAnalyzer
SRCS = $(wildcard ./src/*.cpp)
INC_DIR = include
CC = g++
CFLAGS = -c -Wall -std=c++14

OBJS = $(patsubst ./src/%,./obj/%,$(SRCS:.cpp=.o))

all: $(SRCS) $(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(OUT)
