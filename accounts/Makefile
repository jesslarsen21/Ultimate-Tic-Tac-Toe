CXXFLAGS := -Wall -Werror -g

all: testAccessingDB

accounts: accounts.o testing.o
	g++ $(CXXFLAGS) -o $@ accounts.o testing.o -l sqlite3
	./testing

accounts_test: accounts.o testing.o user.o
	g++ $(CXXFLAGS) -o $@ accounts.o testing.o user.o -l sqlite3
	./accounts_test

testAccessingDB: accounts.o testAccessingDB.o user.o
	g++ $(CXXFLAGS) -o $@ accounts.o testAccessingDB.o user.o -l sqlite3
	./testAccessingDB

accounts.o: accounts.cpp accounts.h user.h
	g++ $(CXXFLAGS) -c accounts.cpp

user.o: user.cpp user.h
	g++ $(CXXFLAGS) -c user.cpp

testing.o: testing.cpp accounts.h user.h
	g++ $(CXXFLAGS) -c testing.cpp

clean:
	-rm -f *.o *~ createDb accounts_test testAccessingDB accounts.db


### for installation in library
### NOTE: libsqlite3-dev is needed
INCS := accounts.h user.h
OBJS := accounts.o user.o
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
