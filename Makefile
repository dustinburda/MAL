CXX_VERSION := c++17
CFLAGS = -Wall -Werror -Wextra

step0:
	g++ -std=$(CXX_VERSION) $(CFLAGS) step0_repl.cpp -o MAL

clean:
	rm -f MAL