EXE = LexicalAnalyzer
SRCS = $(wildcard ./src/*.cpp)
INC_DIR = include
CC = g++
CFLAGS = -c -Wall -Wextra -pedantic -std=c++14

EXEDIR = bin
OUT = $(EXEDIR)/$(EXE)

OBJDIR = obj
OBJS = $(patsubst ./src/%,./$(OBJDIR)/%,$(SRCS:.cpp=.o))

all: $(SRCS) $(OUT)

$(OUT): | $(EXEDIR)

$(EXEDIR):
	mkdir $(EXEDIR)

$(OUT): $(OBJS)
	$(CC) -o $@ $(OBJS)
	
$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(OUT)
