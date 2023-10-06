#compiler settings
CC := g++
CFLAGS := -Wall -Werror -Wextra -pedantic

# directories
SRC := src
OBJ := obj
INCLUDE := include
SRC_FILES := $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/components/*.cpp)

#program settings
PROG := dns

# test modules
TESTS := tests
ARG_TEST := $(TESTS)/argument-tests/arg-test.cpp
TEST_SRC_FILES := $(wildcard tests/**/*.cpp) $(wildcard tests/*.cpp)

.PHONY: clean test

#rules
$(PROG): $(SRC_FILES)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@

clean: 
	rm -f $(PROG) argtest test

test: $(SRC_FILES) $(TEST_SRC_FILES) $(SRC)/$(PROG).cpp
	$(CC) $(CFLAGS) -DTEST -I$(INCLUDE) $^ -o $@
	./$@
