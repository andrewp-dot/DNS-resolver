#compiler settings
CC := g++
CFLAGS := -Wall -Werror -Wextra -pedantic

# directories
SRC := src
OBJ := obj
SRC_FILES := $(wildcard **/*.cpp) $(wildcard $(SRC)/components/*.cpp)

#program settings
PROG := dns


.PHONY: clean

#rules
$(PROG): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean: 
	rm $(PROG)

test:
	echo $(SRC_FILES)