INCS := Server.h ByteBuffer.h Client.h MessagingServer.h MessagingClient.h UTTTServer.h UTTTClient.h logging.h
OBJS := Server.o ByteBuffer.o Client.o MessagingServer.o MessagingClient.o UTTTServer.o UTTTClient.o logging.o
DEPS := $(patsubst %.o,%.d,$(OBJS))
INSTALL_DIR := ../../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
DSTINCS := $(patsubst %,$(INCDIR)/%,$(INCS))
LIBDIR := $(INSTALL_DIR)/lib
LIBFILE := $(LIBDIR)/libuttt.a

CXX := g++
CXXFLAGS := -Wall -I. -I$(INCDIR) -std=c++11 -g -Werror -DDEBUG_LEVEL=15
LDFLAGS := -L$(LIBDIR)
LIBS := -luttt

TARGETS := 

all: $(OBJS) install $(TARGETS) 

build-libs:
	cd $(INSTALL_DIR); make install

test: test-cpp-server
	./test-cpp-server

cpp-echo-server: build-libs cpp-echo-server.o
	$(CXX) $(CXXFLAGS) -o $@ cpp-echo-server.o $(LDFLAGS) $(LIBS)

cpp-echo-server.o: cpp-echo-server.cpp
	$(CXX) $(CXXFLAGS) -c cpp-echo-server.cpp

test-cpp-server: test-cpp-server.o Server.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test-cpp-server.o: test-cpp-server.cpp Server.h
	$(CXX) $(CXXFLAGS) -c test-cpp-server.cpp

$(LIBDIR):
	mkdir $@

install: $(DSTINCS) $(LIBFILE)

$(DSTINCS): $(INCDIR)/%.h: ./%.h
	cp -f $*.h $(INCDIR)/

$(LIBFILE): $(LIBDIR) $(OBJS)
	ar -crsv $(LIBFILE) $(OBJS)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst %.cpp,$*.o $*.d,$<)' $< -MF $@

include $(DEPS)

deps:
	-rm -f *.d

clean:
	-rm -f $(TARGETS) *.o *~ *.d

