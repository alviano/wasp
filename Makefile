#### BUILD modalities
# use 
#   $ make BUILD=release 
# to compile different binaries
BUILD = release
cxxflags.debug = -Wall -Wextra -std=c++11
linkflags.debug = -lm -ldl
cxxflags.trace = -Wall -Wextra -std=c++11 -DTRACE_ON
linkflags.trace = -lm -ldl
cxxflags.tracerelease = -Wall -Wextra -std=c++11 -DTRACE_ON -DNDEBUG -O3
linkflags.tracerelease = -lm -ldl
cxxflags.release = -Wall -Wextra -std=c++11 -DNDEBUG -O3
linkflags.release = -lm -ldl
cxxflags.gprof = -Wall -Wextra -std=c++11 -DNDEBUG -O3 -g -pg
linkflags.gprof = -lm -g -pg -ldl
cxxflags.estats = -Wall -Wextra -std=c++11 -DNDEBUG -DSTATS_ON -DESTATS_ON -O3
linkflags.estats = -lm -ldl

# for g++ <= 4.6
cxxflags.debug0x = -Wall -Wextra -std=c++0x
linkflags.debug0x = -lm -ldl
cxxflags.trace0x = -Wall -Wextra -std=c++0x -DTRACE_ON
linkflags.trace = -lm -ldl
cxxflags.tracerelease0x = -Wall -Wextra -std=c++0x -DTRACE_ON -DNDEBUG -O3
linkflags.tracerelease0x = -lm -ldl
cxxflags.release0x = -Wall -Wextra -std=c++0x -DNDEBUG -O3
linkflags.release0x = -lm -ldl
cxxflags.gprof0x = -Wall -Wextra -std=c++0x -DNDEBUG -O3 -g -pg 
linkflags.gprof0x = -lm -g -pg -ldl
cxxflags.estats0x = -Wall -Wextra -std=c++0x -DNDEBUG -DSTATS_ON -DESTATS_ON -O3
linkflags.estats0x = -lm -ldl
####

SCRIPT = no
scriptsc.perl = -I/usr/local/include -I$(shell perl -MConfig -e 'print $$Config{archlib}')/CORE/
scriptsld.perl = -L$(shell perl -MConfig -e 'print $$Config{archlib}')/CORE/ -L/usr/local/lib/ -lperl
cxxscripts.perl = -DENABLE_PERL

scriptsc.python = $(shell python2.7-config --cflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g" | sed "s/-arch i386//g" | sed "s/-DNDEBUG//g" | sed "s/-Os//g" )
scriptsld.python = $(shell python2.7-config --ldflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g")
cxxscripts.python = -DENABLE_PYTHON

scriptsc.python3 = $(shell python3-config --cflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g" | sed "s/-arch i386//g" | sed "s/-DNDEBUG//g" | sed "s/-Os//g" )
scriptsld.python3 = $(shell python3-config --ldflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g")
cxxscripts.python3 = -DENABLE_PYTHON -DPYTHON_THREE

scriptsc.all = -I/usr/local/include -I$(shell perl -MConfig -e 'print $$Config{archlib}')/CORE/ $(shell python2.7-config --cflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g" | sed "s/-arch i386//g" | sed "s/-DNDEBUG//g" | sed "s/-Os//g" )
scriptsld.all = -L$(shell perl -MConfig -e 'print $$Config{archlib}')/CORE/ -L/usr/local/lib/ -lperl $(shell python2.7-config --ldflags | sed "s/-Wshorten-64-to-32//g" | sed "s/-Wstrict-prototypes//g")
cxxscripts.all = -DENABLE_PERL -DENABLE_PYTHON

scriptsc.no =
scriptld.no =
cxxscripts.no =

SOURCE_DIR = src
BUILD_DIR = build/$(BUILD)
LIB_DIR = $(BUILD_DIR)/wasplib
LIB_SRC_DIR = $(BUILD_DIR)/wasplib/src
LIB_SRC_WASP_DIR = $(BUILD_DIR)/wasplib/wasp

BINARY = $(BUILD_DIR)/wasp
GCC = g++
CXX = $(GCC)
CXXFLAGS = $(cxxflags.$(BUILD))
LINK = $(GCC)
LINKFLAGS = $(linkflags.$(BUILD))

SCRIPT_CFLAGS = $(scriptsc.$(SCRIPT))
SCRIPT_LDFLAGS = $(scriptsld.$(SCRIPT))

CXX_SCRIPTS = $(cxxscripts.$(SCRIPT))

SRCS = $(shell find $(SOURCE_DIR) -name '*.cpp')
SRCSCC = $(shell find $(SOURCE_DIR) -name '*.cc')

LIBCPP = $(patsubst $(SOURCE_DIR)%.cpp,$(LIB_SRC_DIR)%.cpp, $(SRCS))
LIBCC = $(patsubst $(SOURCE_DIR)%.cc,$(LIB_SRC_DIR)%.cc, $(SRCSCC))
OBJS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.o, $(SRCS))
DEPS = $(patsubst $(SOURCE_DIR)%.cpp,$(BUILD_DIR)%.d, $(SRCS))
OBJSCC = $(patsubst $(SOURCE_DIR)%.cc,$(BUILD_DIR)%.o, $(SRCSCC))
DEPSCC = $(patsubst $(SOURCE_DIR)%.cc,$(BUILD_DIR)%.d, $(SRCSCC))

all: $(BINARY)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(SCRIPT_CFLAGS) $(SCRIPT_LDFLAGS) $(CXX_SCRIPTS) -c $< -o $@

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SCRIPT_CFLAGS) $(SCRIPT_LDFLAGS) $(CXX_SCRIPTS) -MM -MT '$(@:.d=.o)' $< -MF $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(SCRIPT_CFLAGS) $(SCRIPT_LDFLAGS) $(CXX_SCRIPTS) -c $< -o $@

$(BUILD_DIR)/%.d: $(SOURCE_DIR)/%.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SCRIPT_CFLAGS) $(SCRIPT_LDFLAGS) $(CXX_SCRIPTS) -MM -MT '$(@:.d=.o)' $< -MF $@
	
$(BINARY): $(OBJS) $(OBJSCC) $(DEPS) $(DEPSCC)
	$(LINK) $(LINKFLAGS) $(SCRIPT_CFLAGS) $(CXX_SCRIPTS) $(LIBS) $(OBJS) $(OBJSCC) $(SCRIPT_LDFLAGS) -o $(BINARY)

static: $(OBJS) $(OBJSCC) $(DEPS) $(DEPSCC)
	$(LINK) $(LINKFLAGS) $(SCRIPT_CFLAGS) $(CXX_SCRIPTS) $(LIBS) $(OBJS) $(OBJSCC) $(SCRIPT_LDFLAGS) -static -o $(BINARY)

$(BUILD_DIR)/wasp.a:
	ar rcs $@ $(OBJS)
	
run: $(BINARY)
	./$(BINARY)

lib: $(BUILD_DIR)/wasp.a
	rm -rf $(LIB_DIR)
	mkdir -p $(LIB_DIR)
	mv $(BUILD_DIR)/wasp.a $(LIB_DIR)/libwasp.a
	cp -r $(SOURCE_DIR) $(LIB_SRC_DIR)
	rm -rf $(LIBCPP) $(LIBCC)
	mv $(LIB_SRC_DIR) $(LIB_SRC_WASP_DIR)
########## Tests

TESTS_DIR = tests

TESTS_TESTER = $(TESTS_DIR)/pyregtest.py

BINARY_WASP = $(BINARY)
TESTS_COMMAND_wasp = $(BINARY_WASP) -n 0 --silent
TESTS_COMMAND_waspweak = $(BINARY_WASP) --silent
TESTS_COMMAND_AllAnswerSets = $(TESTS_COMMAND_wasp)
TESTS_COMMAND_gringo = gringo3 | $(TESTS_COMMAND_wasp)
TESTS_COMMAND_SatModel = $(BINARY_WASP)
TESTS_COMMAND_WeakConstraints = $(TESTS_COMMAND_waspweak)

TESTS_CHECKER_AllAnswerSets = $(TESTS_DIR)/allAnswerSets.checker.py
TESTS_CHECKER_SatModels = $(TESTS_DIR)/satModels.checker.py
TESTS_CHECKER_WeakConstraints = $(TESTS_DIR)/weakConstraints.checker.py

TESTS_REPORT_text = $(TESTS_DIR)/text.report.py

TESTS_DIR_wasp1_AllAnswerSets = $(TESTS_DIR)/wasp1/AllAnswerSets
TESTS_SRC_wasp1_AllAnswerSets = $(sort $(shell find $(TESTS_DIR_wasp1_AllAnswerSets) -name '*.test.py'))
TESTS_OUT_wasp1_AllAnswerSets = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_wasp1_AllAnswerSets))

TESTS_DIR_asp_AllAnswerSetsTight = $(TESTS_DIR)/asp/AllAnswerSets/tight
TESTS_SRC_asp_AllAnswerSetsTight = $(sort $(shell find $(TESTS_DIR_asp_AllAnswerSetsTight) -name '*.test.py'))
TESTS_OUT_asp_AllAnswerSetsTight = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_AllAnswerSetsTight))

TESTS_DIR_asp_AllAnswerSetsNonTight = $(TESTS_DIR)/asp/AllAnswerSets/nontight
TESTS_SRC_asp_AllAnswerSetsNonTight = $(sort $(shell find $(TESTS_DIR_asp_AllAnswerSetsNonTight) -name '*.test.py'))
TESTS_OUT_asp_AllAnswerSetsNonTight = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_AllAnswerSetsNonTight))

TESTS_DIR_asp_AllAnswerSetsAggregates = $(TESTS_DIR)/asp/AllAnswerSets/aggregates
TESTS_SRC_asp_AllAnswerSetsAggregates = $(sort $(shell find $(TESTS_DIR_asp_AllAnswerSetsAggregates) -name '*.test.py'))
TESTS_OUT_asp_AllAnswerSetsAggregates = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_AllAnswerSetsAggregates))

TESTS_DIR_asp_AllAnswerSetsIntensive = $(TESTS_DIR)/asp/AllAnswerSetsIntensive
TESTS_SRC_asp_AllAnswerSetsIntensive = $(sort $(shell find $(TESTS_DIR_asp_AllAnswerSetsIntensive) -name '*.test.py'))
TESTS_OUT_asp_AllAnswerSetsIntensive = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_AllAnswerSetsIntensive))

TESTS_DIR_asp_WeakConstraints = $(TESTS_DIR)/asp/weakConstraints
TESTS_SRC_asp_WeakConstraints = $(sort $(shell find $(TESTS_DIR_asp_WeakConstraints) -name '*.test.py'))
TESTS_OUT_asp_WeakConstraints = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_WeakConstraints))

TESTS_DIR_asp_gringo = $(TESTS_DIR)/asp/gringo
TESTS_SRC_asp_gringo = $(sort $(shell find $(TESTS_DIR_asp_gringo) -name '*.test.py'))
TESTS_OUT_asp_gringo = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_gringo))

TESTS_DIR_asp_cautious = $(TESTS_DIR)/asp/cautious
TESTS_SRC_asp_cautious = $(sort $(shell find $(TESTS_DIR_asp_cautious) -name '*.test.py'))
TESTS_OUT_asp_cautious = $(patsubst %.test.py,%.test.py.text, $(TESTS_SRC_asp_cautious))

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

#tests: tests/wasp1 tests/sat tests/asp

test: tests/sat/Models tests/asp/gringo tests/asp/AllAnswerSets/tight tests/asp/AllAnswerSets/nontight tests/asp/AllAnswerSets/aggregates tests/asp/weakConstraints

tests/wasp1: tests/wasp1/AllAnswerSets

tests/wasp1/AllAnswerSets: $(TESTS_OUT_wasp1_AllAnswerSets)

$(TESTS_OUT_wasp1_AllAnswerSets):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_gringo)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

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

tests/asp: tests/asp/gringo tests/asp/AllAnswerSets/tight tests/asp/AllAnswerSets/nontight tests/asp/AllAnswerSets/aggregates tests/asp/weakConstraints

tests/asp/gringo: $(TESTS_OUT_asp_gringo)
tests/asp/AllAnswerSets/tight: $(TESTS_OUT_asp_AllAnswerSetsTight)
tests/asp/AllAnswerSets/nontight: $(TESTS_OUT_asp_AllAnswerSetsNonTight)
tests/asp/AllAnswerSets/aggregates: $(TESTS_OUT_asp_AllAnswerSetsAggregates)
tests/asp/weakConstraints: $(TESTS_OUT_asp_WeakConstraints)
tests/asp/cautious: $(TESTS_OUT_asp_cautious)

tests/asp/AllAnswerSetsIntensive: $(TESTS_OUT_asp_AllAnswerSetsIntensive)

$(TESTS_OUT_asp_AllAnswerSetsTight):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_AllAnswerSets)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

$(TESTS_OUT_asp_AllAnswerSetsNonTight):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_AllAnswerSets)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

$(TESTS_OUT_asp_AllAnswerSetsAggregates):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_AllAnswerSets)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

$(TESTS_OUT_asp_AllAnswerSetsIntensive):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_AllAnswerSets)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

$(TESTS_OUT_asp_WeakConstraints):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_WeakConstraints)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_WeakConstraints) $(TESTS_REPORT_text)
	
$(TESTS_OUT_asp_cautious):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_WeakConstraints)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)	

$(TESTS_OUT_asp_gringo):
	@$(TESTS_TESTER) "$(TESTS_COMMAND_gringo)" $(patsubst %.test.py.text,%.test.py , $@) $(TESTS_CHECKER_AllAnswerSets) $(TESTS_REPORT_text)

########## Clean

clean-dep:
	rm -f $(DEPS)  $(DEPSCC)
clean: clean-dep
	rm -f $(OBJS)

distclean: clean
	rm -fr $(BUILD_DIR)

-include $(DEPS) $(DEPSCC)
