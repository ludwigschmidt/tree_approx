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

SRCS = treeapprox_binsearch.cc treeapprox_binsearch_main.cc

.PHONY: clean archive

clean:
	rm -rf $(OBJDIR)
	rm -rf $(DEPDIR)
	rm -f treeapprox_binsearch
	rm -f treeapprox_binsearch
	rm -f treeapprox_binsearch.mexa64
	rm -f treeapprox_binsearch.mexmaci64
	rm -f treeapprox.tar.gz

archive:
	mkdir archive-tmp
	tar --transform='s,^\.,treeapprox,' --exclude='.git' --exclude='archive-tmp' -czf archive-tmp/treeapprox.tar.gz .
	mv archive-tmp/treeapprox.tar.gz .
	rm -rf archive-tmp

TREEAPPROX_BINSEARCH_OBJS = treeapprox_binsearch.o

# treeapprox_binsearch executable
TREEAPPROX_BINSEARCH_BIN_OBJS = $(TREEAPPROX_BINSEARCH_OBJS) treeapprox_binsearch_main.o
treeapprox_binsearch: $(TREEAPPROX_BINSEARCH_BIN_OBJS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) -o $@ $^

# gtest
$(OBJDIR)/gtest-all.o: $(GTESTDIR)/src/gtest-all.cc
	$(CXX) $(CXXFLAGS) -I $(GTESTDIR) -c -o $@ $<

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
