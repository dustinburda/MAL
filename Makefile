CXX_VERSION := c++17
CFLAGS = -Wall -Werror -Wextra

SRC_FILES := types.cpp reader.cpp

step0:
	g++ -std=$(CXX_VERSION) $(CFLAGS) step0_repl.cpp -o step0_repl

step1:
	g++ -std=$(CXX_VERSION) $(CFLAGS) step1_read_print.cpp $(SRC_FILES) -o step1_read_print

clean:
	rm -f MAL step0_repl