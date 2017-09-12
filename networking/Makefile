INSTALL_DIR := ../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
LIBDIR := $(INSTALL_DIR)/lib
BINDIR := $(INSTALL_DIR)/bin

CXX := g++
CXXFLAGS := -Wall -Werror -g -std=c++11 -I. -I$(INCDIR)
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt -l3rdparty

TARGETS := echo-client cpp-echo-client cpp-messaging-client cpp-uttt-client

all: build-libs $(TARGETS) install

build-libs:
	cd $(INSTALL_DIR); make install

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/cpp-echo-client: cpp-echo-client $(BINDIR)
	cp -f cpp-echo-client $@

$(BINDIR)/cpp-messaging-client: cpp-messaging-client $(BINDIR)
	cp -f cpp-messaging-client $@

$(BINDIR)/cpp-uttt-client: cpp-uttt-client $(BINDIR)
	cp -f cpp-uttt-client $@

echo-server: echo-server.cpp 
	$(CXX) $(CXXFLAGS) -o $@ $^

echo-client: echo-client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

cpp-echo-client.o: cpp-echo-client.cpp $(INCDIR)/Client.h
	$(CXX) $(CXXFLAGS) -c cpp-echo-client.cpp

cpp-echo-client: cpp-echo-client.o $(LIBDIR)/libuttt.a $(LIBDIR)/lib3rdparty.a
	$(CXX) $(CXXFLAGS) -o $@ cpp-echo-client.o $(LDFLAGS) $(LIBS)

cpp-messaging-client.o: cpp-messaging-client.cpp $(INCDIR)/Client.h $(INCDIR)/MessagingClient.h
	$(CXX) $(CXXFLAGS) -c cpp-messaging-client.cpp

cpp-messaging-client: cpp-messaging-client.o $(LIBDIR)/libuttt.a $(LIBDIR)/lib3rdparty.a
	$(CXX) $(CXXFLAGS) -o $@ cpp-messaging-client.o $(LDFLAGS) $(LIBS)

cpp-uttt-client.o: cpp-uttt-client.cpp $(INCDIR)/Client.h $(INCDIR)/MessagingClient.h $(INCDIR)/UTTTClient.h
	$(CXX) $(CXXFLAGS) -c cpp-uttt-client.cpp

cpp-uttt-client: cpp-uttt-client.o $(LIBDIR)/libuttt.a $(LIBDIR)/lib3rdparty.a
	$(CXX) $(CXXFLAGS) -o $@ cpp-uttt-client.o $(LDFLAGS) $(LIBS)

install: $(BINDIR)/cpp-echo-client $(BINDIR)/cpp-messaging-client $(BINDIR)/cpp-uttt-client

clean:
	-rm -f $(TARGETS) *.o *~
