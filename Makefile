#compiler settings
CC := g++
CFLAGS := -Wall -Werror -Wextra -pedantic --std=c++17

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

.PHONY: clean test log run

#rules
$(PROG): $(SRC_FILES)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@

run: $(PROG)
	./$(PROG) $(ARGS)
	
clean: 
	rm -rf $(PROG) argtest test *.dSYM

test: $(SRC_FILES) $(TEST_SRC_FILES) $(SRC)/$(PROG).cpp
	$(CC) $(CFLAGS) -DTEST -I$(INCLUDE) $^ -o $@
	./$@
	rm -f $@

log: 
	git log --graph > dns_log.txt
