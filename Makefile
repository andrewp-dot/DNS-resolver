#compiler settings
CC := g++
CFLAGS := -Wall -Werror -Wextra -pedantic

# directories
SRC := src
OBJ := obj
INCLUDE := include
SRC_FILES := $(wildcard **/*.cpp) $(wildcard $(SRC)/components/*.cpp)

#program settings
PROG := dns

# test modules
TESTS := tests
ARG_TEST := $(TESTS)/argument-tests/arg-test.cpp

.PHONY: clean argtest test

#rules
$(PROG): $(SRC_FILES)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@

clean: 
	rm -f $(PROG) argtest

argtest: $(ARG_TEST)
	$(CC) $(CFLAGS) $^ -o $@

test: argtest
	./argtest
