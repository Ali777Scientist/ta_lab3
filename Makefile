BISON_FILE = sources/parser.ypp
FLEX_FILE = sources/lexer.lpp
BISON_OUTPUT = sources/build/parser.tab.cpp
FLEX_OUTPUT = sources/build/lex.yy.cpp
HEADER_FILE = sources/build/parser.tab.hpp
EXECUTABLE = parser

all: $(EXECUTABLE)

$(BISON_OUTPUT) $(HEADER_FILE): $(BISON_FILE)
	bison -d $(BISON_FILE) -o $(BISON_OUTPUT)

$(FLEX_OUTPUT): $(FLEX_FILE)
	flex -o $(FLEX_OUTPUT) $(FLEX_FILE)

$(EXECUTABLE): $(BISON_OUTPUT) $(FLEX_OUTPUT) main.cpp
	g++ -o $(EXECUTABLE) $(BISON_OUTPUT) $(FLEX_OUTPUT) main.cpp sources/tree.cpp

clean:
	rm -f $(BISON_OUTPUT) $(FLEX_OUTPUT) $(HEADER_FILE) $(EXECUTABLE)

