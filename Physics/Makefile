# files: testTTT.cpp, TTT.cpp, UTTT.cpp, TTT.h, UTTT.h

# all targets
all: testTTT testUTTT

test: testUTTT 
	./testUTTT

# linker stage explicitly
testUTTT: testUTTT.o UTTT.o TTT.o
	g++ -o $@ $^ 
# $^ = everything on the right of :
# $@ = everything on the left of :	
	./testUTTT	

testTTT: testTTT.o TTT.o
	g++ -o testTTT testTTT.o TTT.o
	./testTTT

testTTT.o: testTTT.cpp TTT.h
	$(CXX) $(CXXFLAGS) -c testTTT.cpp

UTTT.o: UTTT.cpp UTTT.h TTT.h
	$(CXX) $(CXXFLAGS) -c UTTT.cpp

testUTTT.o: testUTTT.cpp UTTT.h TTT.h
	$(CXX) $(CXXFLAGS) -c testUTTT.cpp

TTT.o: TTT.cpp TTT.h
	$(CXX) $(CXXFLAGS) -c TTT.cpp

clean:
# -rm -f (dont show errors) *~ (emacs) 
	-rm -f *~ TTT testTTT testUTTT *.o

### for installation in library
INCS := UTTT.h TTT.h
OBJS := UTTT.o TTT.o
INSTALL_DIR := ../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
LIBDIR := $(INSTALL_DIR)/lib
LIBFILE := $(LIBDIR)/libuttt.a

CXX := g++
CXXFLAGS := -Wall -I. -I$(INCDIR) -std=c++11 -g #-Werror
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt

install: $(LIBDIR) $(INCS) $(OBJS)
	cp -f $(INCS) $(INCDIR)
	ar -crsv $(LIBFILE) $(OBJS)
