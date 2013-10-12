#### BUILD modalities
# use 
#   $ make BUILD=release 
# to compile different binaries
BUILD = debug
cxxflags.debug = \
 -Wall -std=c++11
cxxflags.release = \
 -Wall -std=c++11 -DNDEBUG -O3
cxxflags.gprof = \
 -Wall -std=c++11 -DNDEBUG -O3 -g -pg

# for g++ <= 4.6
cxxflags.debug0x = \
 -Wall -std=c++0x
cxxflags.release0x = \
 -Wall -std=c++0x -DNDEBUG -O3
cxxflags.gprof0x = \
 -Wall -std=c++0x -DNDEBUG -O3 -g -pg 

####

SOURCE_DIR = wasp_src
BUILD_DIR = build/$(BUILD)

BINARY = $(BUILD_DIR)/wasp
GCC = g++
CXX = $(GCC)
CXXFLAGS = $(cxxflags.$(BUILD))
LINK = $(GCC)
LINKFLAGS = -lm -g -pg

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

########## Tests

TESTS_DIR = tests

TESTS_TESTER = $(TESTS_DIR)/pyregtest.py

TESTS_COMMAND_AllAnswerSets = "/home/malvi/workspaces/wasp/wasp/formato_numerico/build/dl -- -silent | /home/malvi/workspaces/wasp/wasp/build/src/wasp_mg" #"gringo | $(BINARY)"

TESTS_CHECKER_AllAnswerSets = $(TESTS_DIR)/allAnswerSets.checker.py

TESTS_REPORT_text = $(TESTS_DIR)/text.report.py

TESTS_DIR_wasp1_AllAnswerSets = $(TESTS_DIR)/wasp1/AllAnswerSets
TESTS_SRC_wasp1_AllAnswerSets = $(shell find $(TESTS_DIR_wasp1_AllAnswerSets) -name '*.test.py')
TESTS_OUT_wasp1_AllAnswerSets = $(patsubst %.test.py,%.test.py., $(TESTS_SRC_wasp1_AllAnswerSets))

tests: tests_wasp1

tests_wasp1: tests_wasp1_AllAnswerSets

tests_wasp1_AllAnswerSets: $(TESTS_OUT_wasp1_AllAnswerSets)

$(TESTS_DIR)/%.test.py.: $(TESTS_DIR)/%.test.py
	@$(TESTS_TESTER) $(TESTS_COMMAND_AllAnswerSets) $< $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

########## Clean

clean:
	rm -f $(OBJS)

distclean: clean
	rm -fr $(BUILD_DIR)

-include $(DEPS)
