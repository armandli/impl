app=test_monte_carlo_integration

SOURCES=test_monte_carlo_integration.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(app)

DEBUG=
OTHER_OPT= -funroll-loops #not working well
OPT= -O3

LIBS=-lgtest -lgtest_main

INCLUDES=-I../ -I./ -I/user/include

CXXFLAGS=-std=c++17 -MD -Wall -Wextra -pthread $(INCLUDES) $(OPT) $(DEBUG)
CXXLINK=$(CXXFLAGS) $(LIBS)

COMPILER=clang++

$(app): %: %.o $(OBJECTS)
	$(COMPILER) $(CXXLINK) $^ -o $@

%.o: %.cpp
	$(COMPILER) $(CXXFLAGS) -c $^

-include $(SOURCES:=.d) $(apps:=.d)

clean:
	-rm $(app) *.o *.d 2> /dev/null
