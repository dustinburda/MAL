CXX_VERSION := c++23
CFLAGS = -Wall -Werror -Wextra

SRC_FILES := ./src/types.cpp ./src/reader.cpp ./src/printer.cpp ./src/environment.cpp ./src/core.cpp
INCLUDE_FILES := ./include/printer.h ./include/reader.h ./include/types.h ./include/environment.h ./include/repfuncs.h ./include/core.h

step0_repl: $(SRC_FILES) $(INCLUDE_FILES) ./src/step0_repl.cpp
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step0_repl.cpp -o step0_repl

step1_read_print: $(SRC_FILES) $(INCLUDE_FILES) ./src/step1_read_print.cpp
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step1_read_print.cpp $(SRC_FILES) -o step1_read_print

step2_eval: $(SRC_FILES) $(INCLUDE_FILES) ./src/step2_eval.cpp
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step2_eval.cpp $(SRC_FILES) -o step2_eval

step3_env: $(SRC_FILES) $(INCLUDE_FILES) ./src/step3_env.cpp
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step3_env.cpp $(SRC_FILES) -o step3_env

step4_if_fn_do: $(SRC_FILES) $(INCLUDE_FILES) ./src/step4_if_fn_do.cpp
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step4_if_fn_do.cpp $(SRC_FILES) -o step4_if_fn_do

step5_tco: $(SRC_FILES) $(INCLUDE_FILES) ./src/step5_tco.cpp
	g++ -std=$(CXX_VERSION) $(CFLAGS) ./src/step4_if_fn_do.cpp $(SRC_FILES) -o step5_tco

clean:
	rm -f MAL step0_repl step1_read_print step2_eval step3_env step4_if_fn_do step5_tco