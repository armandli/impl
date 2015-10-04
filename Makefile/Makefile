app=

SOURCES=

OBJECTS=$(SOURCES:.cpp=.o)

all: $(app)

DEBUG=
LIBS=

CXXFLAGS= -MD -O3 -std=gnu++11 -Wall -Wextra $(LIBS) $(DEBUG)

$(app): %: %.o $(OBJECTS)
	g++ -g -O2 -std=gnu++11 $^ -o $@

-include $(SOURCES:=.d) $(apps:=.d)

clean:
	-rm $(app) *.o *.d 2> /dev/null
