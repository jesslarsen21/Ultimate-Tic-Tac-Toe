INCS := HumanPlayer.h
OBJS := HumanPlayer.o
INSTALL_DIR := ../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
DSTINCS := $(patsubst %,$(INCDIR)/%,$(INCS))
LIBDIR := $(INSTALL_DIR)/lib
LIBFILE := $(LIBDIR)/libuttt.a

CXX := g++
CXXFLAGS := -Wall -I. -I$(INCDIR) -std=c++11 -g -Werror
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt

TARGETS := ai_vs_ai_game hp_test

all: $(OBJS) install $(TARGETS)

build-libs:
	cd $(INSTALL_DIR); make install

ai_vs_ai_game.o: ai_vs_ai_game.cpp $(INCDIR)/UTTT.h $(INCDIR)/TTT.h $(INCDIR)/HumanPlayer.h
ai_vs_ai_game: build-libs ai_vs_ai_game.o $(LIBFILE)
	$(CXX) $(CXXFLAGS) -o $@ ai_vs_ai_game.o $(LDFLAGS) $(LIBS)

HumanPlayer.o: HumanPlayer.cpp HumanPlayer.h

hp_test.o: hp_test.cpp $(INCDIR)/HumanPlayer.h
hp_test: build-libs hp_test.o $(LIBFILE)
	$(CXX) $(CXXFLAGS) -o $@ hp_test.o $(LDFLAGS) $(LIBS)

$(LIBDIR):
	mkdir $@

install: $(DSTINCS) $(LIBFILE)

$(DSTINCS): $(INCDIR)/%.h: ./%.h
	cp -f $*.h $(INCDIR)/

$(LIBFILE): $(LIBDIR) $(OBJS)
	ar -crsv $(LIBFILE) $(OBJS)

clean:
	-rm -f $(TARGETS) *.o *~
