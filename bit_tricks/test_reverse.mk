app=test_reverse

SOURCES=test_reverse.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(app)

DEBUG=
INCLUDES=-I./
OPT=-O3
LIBS=-lgtest -lgtest_main
DEFINES=

CXXFLAGS=-std=c++17 -MD -pedantic -pedantic-errors -Wall -Wextra $(DEFINES) $(INCLUDES) $(OPT) $(DEBUG)
CXXLINKS=$(CXXFLAGS) $(LIBS)

COMPILER=g++

$(app): %: %.o $(OBJECTS)
	$(COMPILER) $(CXXLINKS) $^ -o $@

%.o: %.cpp
	$(COMPILER) $(CXXFLAGS) -c $^

-include $(SOURCES:=.d) $(apps:=.d)

clean:
	-rm $(app) *.o *.d 2> /dev/null
