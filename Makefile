SOURCE_DIR := wasp_src

# use 
#   $ make BUILD=benchmarks 
# to compile different binaries
BUILD := debug
cxxflags.debug := -Wall -std=c++11 $(CODE)
cxxflags.benchmarks := -Wall -std=c++11 $(CODE) -DNDEBUG -O3

# for g++ <= 4.6
cxxflags.debug0x := -Wall -std=c++0x $(CODE)
cxxflags.benchmarks0x := -Wall -std=c++0x $(CODE) -DNDEBUG -O3

BUILD_DIR = build/$(BUILD)

BINARY=$(BUILD_DIR)/wasp
GCC=g++
CXX=$(GCC)
CXXFLAGS := ${cxxflags.${BUILD}}
LINK=$(GCC)
LINKFLAGS=-lm

SRCS = $(shell find $(SOURCE_DIR) -name '*.cpp')

OBJS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SRCS))
DEPS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.d, $(SRCS))

all: $(BINARY) 

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MM -MT '$(@:.d=.o)' $< -MF $@
	
$(BINARY): $(OBJS) $(DEPS)
	$(LINK) $(LINKFLAGS) $(LIBS) $(OBJS) -o $(BINARY)

static: $(OBJS) $(DEPS)
	$(LINK) $(LINKFLAGS) $(LIBS) $(OBJS) -static -o $(BINARY)

run: $(BINARY)
	./$(BINARY)

########## Clean

clean:
	rm -f $(OBJS)

distclean: clean
	rm -fr $(BUILD_DIR)

-include $(DEPS)
