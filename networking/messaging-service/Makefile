INSTALL_DIR := ../../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
LIBDIR := $(INSTALL_DIR)/lib
BINDIR := $(INSTALL_DIR)/bin

CXX := g++
CXXFLAGS := -Wall -I. -I$(INCDIR) -std=c++11 -g -Werror
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt

TARGETS := messaging-service

all: build-libs $(TARGETS) install

build-libs:
	cd $(INSTALL_DIR); make install

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/messaging-service: messaging-service $(BINDIR)
	cp -f messaging-service $@

messaging-service: messaging-service.o $(LIBDIR)/libuttt.a
	$(CXX) $(CXXFLAGS) -o $@ messaging-service.o $(LDFLAGS) $(LIBS)

messaging-service.o: messaging-service.cpp
	$(CXX) $(CXXFLAGS) -c messaging-service.cpp

install: $(BINDIR)/messaging-service

clean:
	-rm -f $(TARGETS) *.o *~

