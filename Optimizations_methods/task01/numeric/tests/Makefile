# Makefie
# Building and running unit tests rules.
# Vladimir Rutsky <altsysrq@gmail.com>
# 23.02.2009

CC          := g++
CFLAGS      += -W -Wall -std=c++98 -pedantic -I/usr/src/boost_1_38_0
LDFLAGS     +=
BINARY_NAME := ./run_tests
SOURCE      := numeric_tests.cpp

all,$(BINARY_NAME):
	$(CC) -o $(BINARY_NAME) $(CFLAGS) $(LDFLAGS) $(SOURCE)

run: $(BINARY_NAME)
	$(BINARY_NAME)

clean:
	rm -rf $(BINARY_NAME)
