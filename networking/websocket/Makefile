INCS := base64.hpp HttpSegments.h sha1.hpp stdint.hpp utf8.hpp WebSocketServer.h WebSocketUtils.h WebSocketValidator.h
OBJS := WebSocketValidator.o WebSocketUtils.o
INSTALL_DIR := ../../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
LIBDIR := $(INSTALL_DIR)/lib
LIBFILE := $(LIBDIR)/libuttt.a

CXX := g++
CXXFLAGS := -Wall -Werror -g -std=c++11 -I$(INCDIR)

all: $(OBJS) install

$(LIBDIR):
	mkdir $@

WebSocketValidator.o: WebSocketValidator.cpp $(INCS)
WebSocketUtils.o: WebSocketUtils.cpp $(INCS)

install: $(LIBDIR) $(INCS) $(OBJS)
	cp -f $(INCS) $(INCDIR)
	ar -crsv $(LIBFILE) $(OBJS) 

clean:
	-rm -f *.o *~

