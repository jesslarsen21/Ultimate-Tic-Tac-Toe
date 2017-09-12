INSTALL_DIR := ../../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
LIBDIR := $(INSTALL_DIR)/lib
BINDIR := $(INSTALL_DIR)/bin

CXX := g++
CXXFLAGS := -Wall -I. -I$(INCDIR) -std=c++11 -g -Werror
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt

TARGETS := echo-service

all: build-libs $(TARGETS) install

build-libs:
	cd $(INSTALL_DIR); make install

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/echo-service: echo-service $(BINDIR)
	cp -f echo-service $@

echo-service: echo-service.o $(LIBDIR)/libuttt.a
	$(CXX) $(CXXFLAGS) -o $@ echo-service.o $(LDFLAGS) $(LIBS)

echo-service.o: echo-service.cpp
	$(CXX) $(CXXFLAGS) -c echo-service.cpp

install: $(BINDIR)/echo-service

clean:
	-rm -f $(TARGETS) *.o *~

