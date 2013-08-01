BINARY=wasp
GCC=g++
CXX=$(GCC)
CXXFLAGS=-Wall -std=c++11 $(CODE)
LINK=$(GCC)
LINKFLAGS=-lm

SRCS = $(wildcard *.cpp) $(wildcard learning/*.cpp) $(wildcard stl/*.cpp)

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

all: $(BINARY) 
	
%.d: %.cpp
	$(CXX) $(CXXFLAGS) -M $< >$@
	
$(BINARY): $(OBJS)
	$(LINK) $(LINKFLAGS) $(LIBS) $(OBJS) -o $(BINARY)

static: $(OBJS)
	$(LINK) $(LINKFLAGS) $(LIBS) $(OBJS) -static -o $(BINARY)

run: $(BINARY)
	./$(BINARY)

########## Clean

clean:
	rm -f *.o a.out
	rm -f gmon.out

distclean: clean
	rm -f $(BINARY)
	rm -f *.d

-include $(DEPS)

