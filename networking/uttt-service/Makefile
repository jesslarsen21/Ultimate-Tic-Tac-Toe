INSTALL_DIR := ../../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
LIBDIR := $(INSTALL_DIR)/lib
BINDIR := $(INSTALL_DIR)/bin

CXX := g++
CXXFLAGS := -Wall -I. -I$(INCDIR) -std=c++11 -g -Werror
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt -lsqlite3

TARGETS := uttt-service

all: build-libs $(TARGETS) install

build-libs:
	cd $(INSTALL_DIR); make install

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/uttt-service: uttt-service $(BINDIR)
	cp -f uttt-service $@

uttt-service: uttt-service.o $(LIBDIR)/libuttt.a
	$(CXX) $(CXXFLAGS) -o $@ uttt-service.o $(LDFLAGS) $(LIBS)

uttt-service.o: uttt-service.cpp
	$(CXX) $(CXXFLAGS) -c uttt-service.cpp

install: $(BINDIR)/uttt-service

clean:
	-rm -f $(TARGETS) *.o *~

