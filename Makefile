# Ludwig Schmidt (ludwigschmidt2@gmail.com) 2013
#
# This makefile is based on http://make.paulandlesley.org/autodep.html .

CXX = g++
MEX = mex
CXXFLAGS = -Wall -Wextra -O3 -fPIC
#CXXFLAGS = -Wall -Wextra -g -fPIC
MEXCXXFLAGS = -Wall -Wextra -O3
GTESTDIR = /usr/src/gtest

SRCDIR = src
DEPDIR = .deps
OBJDIR = obj

SRCS = treeapprox_binsearch.cc treeapprox_binsearch_main.cc treeapprox_binsearch_mex_wrapper.cc treeapprox_binsearch_test.cc treeexact_fulltable.cc treeexact_fulltable_main.cc treeexact_fulltable_mex_wrapper.cc treeexact_fulltable_test.cc treeexact_smalltable.cc treeexact_smalltable_main.cc treeexact_smalltable_mex_wrapper.cc treeexact_smalltable_test.cc rearrange_wavedec2_test.cc treeexact_smalltable_wvtree.cc treeexact_smalltable_wvtree_test.cc treeexact_smalltable_wvtree_mex_wrapper.cc treeexact_fulltable_wvtree.cc treeexact_fulltable_wvtree_test.cc treeexact_fulltable_wvtree_mex_wrapper.cc

.PHONY: clean archive

clean:
	rm -rf $(OBJDIR)
	rm -rf $(DEPDIR)
	rm -f treeapprox_binsearch
	rm -f treeapprox_binsearch_test
	rm -f treeapprox_binsearch.mexa64
	rm -f treeapprox_binsearch.mexmaci64
	rm -f treeexact_fulltable
	rm -f treeexact_fulltable_test
	rm -f treeexact_fulltable.mexa64
	rm -f treeexact_fulltable.mexmaci64
	rm -f treeexact_smalltable
	rm -f treeexact_smalltable_test
	rm -f treeexact_smalltable.mexa64
	rm -f treeexact_smalltable.mexmaci64
	rm -f treeexact_smalltable_wvtree
	rm -f treeexact_smalltable_wvtree_test
	rm -f treeexact_smalltable_wvtree.mexa64
	rm -f treeexact_smalltable_wvtree.mexmaci64
	rm -f treeexact_fulltable_wvtree
	rm -f treeexact_fulltable_wvtree_test
	rm -f treeexact_fulltable_wvtree.mexa64
	rm -f treeexact_fulltable_wvtree.mexmaci64
	rm -f rearrange_wavedec2_test
	rm -f rearrange_wavedec2.mexa64
	rm -f rearrange_wavedec2.mexmaci64
	rm -f treeapprox.tar.gz

archive:
	mkdir archive-tmp
	tar --transform='s,^\.,treeapprox,' --exclude='.git' --exclude='archive-tmp' -czf archive-tmp/treeapprox.tar.gz .
	mv archive-tmp/treeapprox.tar.gz .
	rm -rf archive-tmp

run_tests: run_treeapprox_binsearch_test run_treeexact_fulltable_test run_treeexact_smalltable_test run_treeexact_smalltable_wvtree_test run_treeexact_fulltable_wvtree_test run_rearrange_wavedec2_test

mexfiles: treeapprox_binsearch_mexfile treeexact_fulltable_mexfile treeexact_smalltable_mexfile treeexact_smalltable_wvtree_mexfile treeexact_fulltable_wvtree_mexfile rearrange_wavedec2_mexfile

# gtest
$(OBJDIR)/gtest-all.o: $(GTESTDIR)/src/gtest-all.cc
	$(CXX) $(CXXFLAGS) -I $(GTESTDIR) -c -o $@ $<


# treeapprox_binsearch

TREEAPPROX_BINSEARCH_OBJS = treeapprox_binsearch.o

# treeapprox_binsearch executable
TREEAPPROX_BINSEARCH_BIN_OBJS = $(TREEAPPROX_BINSEARCH_OBJS) treeapprox_binsearch_main.o
treeapprox_binsearch: $(TREEAPPROX_BINSEARCH_BIN_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^

# treeapprox_binsearch tests
TREEAPPROX_BINSEARCH_TEST_OBJS = $(TREEAPPROX_BINSEARCH_OBJS) treeapprox_binsearch_test.o gtest-all.o
treeapprox_binsearch_test: $(TREEAPPROX_BINSEARCH_TEST_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

run_treeapprox_binsearch_test: treeapprox_binsearch_test
	./treeapprox_binsearch_test

# treeapprox_binsearch MEX file
TREEAPPROX_BINSEARCH_MEXFILE_OBJS = $(TREEAPPROX_BINSEARCH_OBJS)
TREEAPPROX_BINSEARCH_MEXFILE_SRC = treeapprox_binsearch_mex_wrapper.cc
TREEAPPROX_BINSEARCH_MEXFILE_SRC_DEPS = $(TREEAPPROX_BINSEARCH_MEXFILE_SRC) mex_helper.h treeapprox_binsearch.h

treeapprox_binsearch_mexfile: $(TREEAPPROX_BINSEARCH_MEXFILE_OBJS:%=$(OBJDIR)/%) $(TREEAPPROX_BINSEARCH_MEXFILE_SRC_DEPS:%=$(SRCDIR)/%)
	$(MEX) -v CXXFLAGS="\$$CXXFLAGS $(MEXCXXFLAGS)" -output treeapprox_binsearch $(SRCDIR)/$(TREEAPPROX_BINSEARCH_MEXFILE_SRC) $(TREEAPPROX_BINSEARCH_MEXFILE_OBJS:%=$(OBJDIR)/%)


# treeexact_fulltable

TREEEXACT_FULLTABLE_OBJS = treeexact_fulltable.o

# treeexact_fulltable executable
TREEEXACT_FULLTABLE_BIN_OBJS = $(TREEEXACT_FULLTABLE_OBJS) treeexact_fulltable_main.o
treeexact_fulltable: $(TREEEXACT_FULLTABLE_BIN_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^

# treeexact_fulltable tests
TREEEXACT_FULLTABLE_TEST_OBJS = $(TREEEXACT_FULLTABLE_OBJS) treeexact_fulltable_test.o gtest-all.o
treeexact_fulltable_test: $(TREEEXACT_FULLTABLE_TEST_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

run_treeexact_fulltable_test: treeexact_fulltable_test
	./treeexact_fulltable_test

# treeexact_fulltable MEX file
TREEEXACT_FULLTABLE_MEXFILE_OBJS = $(TREEEXACT_FULLTABLE_OBJS)
TREEEXACT_FULLTABLE_MEXFILE_SRC = treeexact_fulltable_mex_wrapper.cc
TREEEXACT_FULLTABLE_MEXFILE_SRC_DEPS = $(TREEEXACT_FULLTABLE_MEXFILE_SRC) mex_helper.h treeexact_fulltable.h

treeexact_fulltable_mexfile: $(TREEEXACT_FULLTABLE_MEXFILE_OBJS:%=$(OBJDIR)/%) $(TREEEXACT_FULLTABLE_MEXFILE_SRC_DEPS:%=$(SRCDIR)/%)
	$(MEX) -v CXXFLAGS="\$$CXXFLAGS $(MEXCXXFLAGS)" -output treeexact_fulltable $(SRCDIR)/$(TREEEXACT_FULLTABLE_MEXFILE_SRC) $(TREEEXACT_FULLTABLE_MEXFILE_OBJS:%=$(OBJDIR)/%)


# treeexact_smalltable

TREEEXACT_SMALLTABLE_OBJS = treeexact_smalltable.o

# treeexact_smalltable executable
TREEEXACT_SMALLTABLE_BIN_OBJS = $(TREEEXACT_SMALLTABLE_OBJS) treeexact_smalltable_main.o
treeexact_smalltable: $(TREEEXACT_SMALLTABLE_BIN_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^

# treeexact_smalltable tests
TREEEXACT_SMALLTABLE_TEST_OBJS = $(TREEEXACT_SMALLTABLE_OBJS) treeexact_smalltable_test.o gtest-all.o
treeexact_smalltable_test: $(TREEEXACT_SMALLTABLE_TEST_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

run_treeexact_smalltable_test: treeexact_smalltable_test
	./treeexact_smalltable_test

# treeexact_smalltable MEX file
TREEEXACT_SMALLTABLE_MEXFILE_OBJS = $(TREEEXACT_SMALLTABLE_OBJS)
TREEEXACT_SMALLTABLE_MEXFILE_SRC = treeexact_smalltable_mex_wrapper.cc
TREEEXACT_SMALLTABLE_MEXFILE_SRC_DEPS = $(TREEEXACT_SMALLTABLE_MEXFILE_SRC) mex_helper.h treeexact_smalltable.h

treeexact_smalltable_mexfile: $(TREEEXACT_SMALLTABLE_MEXFILE_OBJS:%=$(OBJDIR)/%) $(TREEEXACT_SMALLTABLE_MEXFILE_SRC_DEPS:%=$(SRCDIR)/%)
	$(MEX) -v CXXFLAGS="\$$CXXFLAGS $(MEXCXXFLAGS)" -output treeexact_smalltable $(SRCDIR)/$(TREEEXACT_SMALLTABLE_MEXFILE_SRC) $(TREEEXACT_SMALLTABLE_MEXFILE_OBJS:%=$(OBJDIR)/%)


# treeexact_smalltable_wvtree

TREEEXACT_SMALLTABLE_WVTREE_OBJS = treeexact_smalltable_wvtree.o

# treeexact_smalltable_wvtree tests
TREEEXACT_SMALLTABLE_WVTREE_TEST_OBJS = $(TREEEXACT_SMALLTABLE_WVTREE_OBJS) treeexact_smalltable_wvtree_test.o gtest-all.o
treeexact_smalltable_wvtree_test: $(TREEEXACT_SMALLTABLE_WVTREE_TEST_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

run_treeexact_smalltable_wvtree_test: treeexact_smalltable_wvtree_test
	./treeexact_smalltable_wvtree_test

# treeexact_smalltable_wvtree MEX file
TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_OBJS = $(TREEEXACT_SMALLTABLE_WVTREE_OBJS)
TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_SRC = treeexact_smalltable_wvtree_mex_wrapper.cc
TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_SRC_DEPS = $(TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_SRC) mex_helper.h treeexact_smalltable_wvtree.h

treeexact_smalltable_wvtree_mexfile: $(TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_OBJS:%=$(OBJDIR)/%) $(TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_SRC_DEPS:%=$(SRCDIR)/%)
	$(MEX) -v CXXFLAGS="\$$CXXFLAGS $(MEXCXXFLAGS)" -output treeexact_smalltable_wvtree $(SRCDIR)/$(TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_SRC) $(TREEEXACT_SMALLTABLE_WVTREE_MEXFILE_OBJS:%=$(OBJDIR)/%)


# treeexact_fulltable_wvtree

TREEEXACT_FULLTABLE_WVTREE_OBJS = treeexact_fulltable_wvtree.o

# treeexact_fulltable_wvtree tests
TREEEXACT_FULLTABLE_WVTREE_TEST_OBJS = $(TREEEXACT_FULLTABLE_WVTREE_OBJS) treeexact_fulltable_wvtree_test.o gtest-all.o
treeexact_fulltable_wvtree_test: $(TREEEXACT_FULLTABLE_WVTREE_TEST_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

run_treeexact_fulltable_wvtree_test: treeexact_fulltable_wvtree_test
	./treeexact_fulltable_wvtree_test

# treeexact_fulltable_wvtree MEX file
TREEEXACT_FULLTABLE_WVTREE_MEXFILE_OBJS = $(TREEEXACT_FULLTABLE_WVTREE_OBJS)
TREEEXACT_FULLTABLE_WVTREE_MEXFILE_SRC = treeexact_fulltable_wvtree_mex_wrapper.cc
TREEEXACT_FULLTABLE_WVTREE_MEXFILE_SRC_DEPS = $(TREEEXACT_FULLTABLE_WVTREE_MEXFILE_SRC) mex_helper.h treeexact_fulltable_wvtree.h

treeexact_fulltable_wvtree_mexfile: $(TREEEXACT_FULLTABLE_WVTREE_MEXFILE_OBJS:%=$(OBJDIR)/%) $(TREEEXACT_FULLTABLE_WVTREE_MEXFILE_SRC_DEPS:%=$(SRCDIR)/%)
	$(MEX) -v CXXFLAGS="\$$CXXFLAGS $(MEXCXXFLAGS)" -output treeexact_fulltable_wvtree $(SRCDIR)/$(TREEEXACT_FULLTABLE_WVTREE_MEXFILE_SRC) $(TREEEXACT_FULLTABLE_WVTREE_MEXFILE_OBJS:%=$(OBJDIR)/%)


# rearrange_wavedec2

# rearrange_wavedec2 tests
REARRANGE_WAVEDEC2_TEST_OBJS = rearrange_wavedec2_test.o gtest-all.o
rearrange_wavedec2_test: $(REARRANGE_WAVEDEC2_TEST_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

run_rearrange_wavedec2_test: rearrange_wavedec2_test
	./rearrange_wavedec2_test

# rearrange_wavedec2 MEX file
REARRANGE_WAVEDEC2_MEXFILE_OBJS = $(TREEEXACT_SMALLTABLE_WVTREE_OBJS)
REARRANGE_WAVEDEC2_MEXFILE_SRC = rearrange_wavedec2_mex_wrapper.cc
REARRANGE_WAVEDEC2_MEXFILE_SRC_DEPS = $(REARRANGE_WAVEDEC2_MEXFILE_SRC) mex_helper.h rearrange_wavedec2.h

rearrange_wavedec2_mexfile: $(REARRANGE_WAVEDEC2_MEXFILE_OBJS:%=$(OBJDIR)/%) $(REARRANGE_WAVEDEC2_MEXFILE_SRC_DEPS:%=$(SRCDIR)/%)
	$(MEX) -v CXXFLAGS="\$$CXXFLAGS $(MEXCXXFLAGS)" -output rearrange_wavedec2 $(SRCDIR)/$(REARRANGE_WAVEDEC2_MEXFILE_SRC) $(REARRANGE_WAVEDEC2_MEXFILE_OBJS:%=$(OBJDIR)/%)


$(OBJDIR)/%.o: $(SRCDIR)/%.cc
  # Create the directory the current target lives in.
	@mkdir -p $(@D)
  # Compile and generate a dependency file.
  # See http://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html .
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<
  # Move dependency file to dependency file directory.
  # Create the dependency file directory if necessary.
	@mkdir -p $(DEPDIR)
	@mv $(OBJDIR)/$*.d $(DEPDIR)/$*.d

# Include the generated dependency files.
# The command replaces each file name in SRCS with its dependency file.
# See http://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html#Substitution-Refs for the GNU make details.
-include $(SRCS:%.cc=$(DEPDIR)/%.d)
