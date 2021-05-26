#
# Makefile
#

# set the executable name
EXEC=app

CC=g++
CFLAGS+= -std=c++17 -Wall -g -Wno-unused-variable -Wno-unused-but-set-variable
CFLAGS+= -Iinclude
LDLIBS:= -lm

ODIR:=obj

SRC := $(wildcard *.cpp)
OBJS = $(patsubst %,$(ODIR)/%,$(SRC:.cpp=.o))

all: $(EXEC)

-include $(ODIR)/*.d

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(ODIR)/%.o: %.cpp | $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MF $(@:.o=.d)

$(ODIR):
	mkdir $@

clean:
	$(RM) $(EXEC)
	$(RM) -rf $(ODIR)

test: $(EXEC)
	baygon -v -t test.json ./$(EXEC)

testf: $(EXEC)
	baygon -v -t testf.json ./$(EXEC)

format: main.cpp
	clang-format -i main.cpp

.PHONY:	clean all test
.DEFAULT: all
