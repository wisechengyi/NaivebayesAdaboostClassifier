####################################################################
# File   : Makefile - automates building of the project.             #
# Author : Tim Sally                                                 #
######################################################################

CXX        = g++
CXX_FLAGS  = -g -ansi -Wall 


# - Files 
SRCS1       = adaboost.cpp
SRCS2      = naivebayes.cpp
OBJS1       = $(SRCS1:.cxx=.o)
OBJS2       = $(SRCS2:.cxx=.o)

TARGETS1    = adaboost
TARGETS2    = naive_bayes

.PHONY: $(TARGETS)

# --- Suffix Rules 

.SUFFIXES: 
.SUFFIXES: .cxx .o

.cxx.o:
	$(CXX) -c $(CXX_FLAGS) $< 

# --- Targets
all: $(TARGETS1) $(TARGETS2)

$(TARGETS1): $(OBJS1)
	$(CXX) -o $@ $^ $(LD_FLAGS)

$(TARGETS2): $(OBJS2)
	$(CXX) -o $@ $^ $(LD_FLAGS)

clean:
	rm -f naive_bayes adaboost
