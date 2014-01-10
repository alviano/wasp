#### BUILD modalities
# use 
#   $ make BUILD=release 
# to compile different binaries
BUILD = release
cxxflags.debug = \
 -Wall -std=c++11
linkflags.debug = \
 -lm
cxxflags.trace = \
 -Wall -std=c++11 -DTRACE_ON
linkflags.trace = \
 -lm
cxxflags.release = \
 -Wall -std=c++11 -DNDEBUG -O3
linkflags.release = \
 -lm
cxxflags.gprof = \
 -Wall -std=c++11 -DNDEBUG -O3 -g -pg
linkflags.gprof = \
 -lm -g -pg
cxxflags.stats = \
 -Wall -std=c++11 -DNDEBUG -DSTATS_ON -O3
linkflags.stats = \
 -lm -g -pg

# for g++ <= 4.6
cxxflags.debug0x = \
 -Wall -std=c++0x
linkflags.debug0x = \
 -lm
cxxflags.trace0x = \
 -Wall -std=c++0x -DTRACE_ON
linkflags.trace = \
 -lm
cxxflags.release0x = \
 -Wall -std=c++0x -DNDEBUG -O3
linkflags.release0x = \
 -lm
cxxflags.gprof0x = \
 -Wall -std=c++0x -DNDEBUG -O3 -g -pg 
linkflags.gprof0x = \
 -lm -g -pg
cxxflags.stats0x = \
 -Wall -std=c++0x -DNDEBUG -DSTATS_ON -O3
linkflags.stats0x = \
 -lm -g -pg
####

SOURCE_DIR = src
BUILD_DIR = build/$(BUILD)

BINARY = $(BUILD_DIR)/wasp
GCC = g++
CXX = $(GCC)
CXXFLAGS = $(cxxflags.$(BUILD))
LINK = $(GCC)
LINKFLAGS = $(linkflags.$(BUILD))

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

TESTS_COMMAND_AllAnswerSets = $(BINARY) -n 0 --silent #gringo | $(BINARY)
TESTS_COMMAND_SatModel = $(BINARY)

TESTS_CHECKER_AllAnswerSets = $(TESTS_DIR)/allAnswerSets.checker.py
TESTS_CHECKER_SatModels = $(TESTS_DIR)/satModels.checker.py

TESTS_REPORT_text = $(TESTS_DIR)/text.report.py

TESTS_DIR_wasp1_AllAnswerSets = $(TESTS_DIR)/wasp1/AllAnswerSets
TESTS_SRC_wasp1_AllAnswerSets = $(sort $(shell find $(TESTS_DIR_wasp1_AllAnswerSets) -name '*.test.py'))
TESTS_OUT_wasp1_AllAnswerSets = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_wasp1_AllAnswerSets))

TESTS_DIR_asp_AllAnswerSets = $(TESTS_DIR)/asp/AllAnswerSets
TESTS_SRC_asp_AllAnswerSets = $(sort $(shell find $(TESTS_DIR_asp_AllAnswerSets) -name '*.test.py'))
TESTS_OUT_asp_AllAnswerSets = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_AllAnswerSets))

TESTS_DIR_asp_AllAnswerSetsIntensive = $(TESTS_DIR)/asp/AllAnswerSetsIntensive
TESTS_SRC_asp_AllAnswerSetsIntensive = $(sort $(shell find $(TESTS_DIR_asp_AllAnswerSetsIntensive) -name '*.test.py'))
TESTS_OUT_asp_AllAnswerSetsIntensive = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_AllAnswerSetsIntensive))

TESTS_DIR_sat_Models = $(TESTS_DIR)/sat/Models
TESTS_SRC_sat_Models = $(sort $(shell find $(TESTS_DIR_sat_Models) -name '*.test.py'))
TESTS_OUT_sat_Models = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_sat_Models))

TESTS_DIR_sat_Intensive = $(TESTS_DIR)/sat/Intensive
TESTS_SRC_sat_Intensive = $(sort $(shell find $(TESTS_DIR_sat_Intensive) -name '*.test.py'))
TESTS_OUT_sat_Intensive = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_sat_Intensive))

TESTS_DIR_sat_Intensive2 = $(TESTS_DIR)/sat/Intensive2
TESTS_SRC_sat_Intensive2 = $(sort $(shell find $(TESTS_DIR_sat_Intensive2) -name '*.test.py'))
TESTS_OUT_sat_Intensive2 = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_sat_Intensive2))

TESTS_DIR_sat_Intensive3 = $(TESTS_DIR)/sat/Intensive3
TESTS_SRC_sat_Intensive3 = $(sort $(shell find $(TESTS_DIR_sat_Intensive3) -name '*.test.py'))
TESTS_OUT_sat_Intensive3 = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_sat_Intensive3))

tests: tests/wasp1 tests/sat tests/asp

tests/wasp1: tests/wasp1/AllAnswerSets

tests/wasp1/AllAnswerSets: $(TESTS_OUT_wasp1_AllAnswerSets)

$(TESTS_OUT_wasp1_AllAnswerSets):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_AllAnswerSets)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

tests/sat: tests/sat/Models tests/sat/Intensive tests/sat/Intensive2 tests/sat/Intensive3

tests/sat/Models: $(TESTS_OUT_sat_Models)

$(TESTS_OUT_sat_Models):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_SatModel)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_SatModels) $(TESTS_REPORT_text)

tests/sat/Intensive: $(TESTS_OUT_sat_Intensive)

$(TESTS_OUT_sat_Intensive):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_SatModel)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_SatModels) $(TESTS_REPORT_text)

tests/sat/Intensive2: $(TESTS_OUT_sat_Intensive2)

$(TESTS_OUT_sat_Intensive2):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_SatModel)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_SatModels) $(TESTS_REPORT_text)
	
tests/sat/Intensive3: $(TESTS_OUT_sat_Intensive3)

$(TESTS_OUT_sat_Intensive3):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_SatModel)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_SatModels) $(TESTS_REPORT_text)

tests/asp: tests/asp/AllAnswerSets tests/asp/AllAnswerSetsIntensive

tests/asp/AllAnswerSets: $(TESTS_OUT_asp_AllAnswerSets)

tests/asp/AllAnswerSetsIntensive: $(TESTS_OUT_asp_AllAnswerSetsIntensive)

$(TESTS_OUT_asp_AllAnswerSets):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_AllAnswerSets)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

$(TESTS_OUT_asp_AllAnswerSetsIntensive):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_AllAnswerSets)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

########## Clean

clean-dep:
	rm -f $(DEPS)
clean: clean-dep
	rm -f $(OBJS)

distclean: clean
	rm -fr $(BUILD_DIR)

-include $(DEPS)
