INCS := AIPlayer.h debug.h HumanPlayer.h Player.h
OBJS := AIPlayer.o HumanPlayer.o Player.o
INSTALL_DIR := ../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
LIBDIR := $(INSTALL_DIR)/lib
LIBFILE := $(LIBDIR)/libuttt.a
TARGETS := # ai_vs_ai_game

CXX := g++
CXXFLAGS := -Wall -I. -I$(INCDIR) -std=c++11 -g #-Werror
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt

all: build-libs $(TARGETS) install

build-libs:
	cd $(INSTALL_DIR); make install

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/ai_vs_ai_game: ai_vs_ai_game $(BINDIR)
	cp -f ai_vs_ai_game $@

ai_vs_ai_game: ai_vs_ai_game.o $(LIBDIR)/libuttt.a
	$(CXX) $(CXXFLAGS) -o $@ ai_vs_ai_game.o $(LDFLAGS) $(LIBS)

ai_vs_ai_game.o: ai_vs_ai_game.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c ai_vs_ai_game.cpp

AIPlayer.o: AIPlayer.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c AIPlayer.cpp

HumanPlayer.o: HumanPlayer.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c HumanPlayer.cpp

Player.o: Player.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c Player.cpp


install: $(LIBDIR) $(INCS) $(OBJS)
	cp -f $(INCS) $(INCDIR)
	ar -crsv $(LIBFILE) $(OBJS)

clean:
	-rm -f $(TARGETS) *.o *~

