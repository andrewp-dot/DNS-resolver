#compiler settings
CC := g++
CFLAGS := -Wall -Werror -Wextra -pedantic --std=c++17

#program settings
PROG := dns

# directories
SRC := src
OBJ := obj
INCLUDE := include
COMPONENTS := $(SRC)/components
# SRC_FILES := $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/components/*.cpp)
SRC_FILES := $(SRC)/$(PROG).cpp $(COMPONENTS)/Connection.cpp $(COMPONENTS)/Error.cpp $(COMPONENTS)/InputParser.cpp $(COMPONENTS)/Message.cpp $(COMPONENTS)/Query.cpp

.PHONY: clean test log run

#rules
$(PROG): $(SRC_FILES)
	$(CC) $(CFLAGS) -I$(INCLUDE) $(SRC_FILES) -o $(PROG)

run: $(PROG)
	./$(PROG) $(ARGS)
	
clean: 
	rm -rf $(PROG) test *.dSYM

test: $(PROG)
	@cp $(PROG) ./tests/$(PROG)
	@python3 ./tests/tests.py
	@rm ./tests/$(PROG)
	@rm -r ./tests/__pycache__


log: 
	git log --graph > dns_log.txt
