CXX_VERSION := c++23
CFLAGS = -Wall -Werror -Wextra

SRC_FILES := ./src/types.cpp ./src/reader.cpp ./src/printer.cpp
INCLUDE_FILES := ./include/printer.h ./include/reader.h ./include/types.h

step0_repl:
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step0_repl.cpp -o step0_repl

step1_read_print: $(SRC_FILES) $(INCLUDE_FILES)
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step1_read_print.cpp $(SRC_FILES) -o step1_read_print

clean:
	rm -f MAL step0_repl step1_read_print