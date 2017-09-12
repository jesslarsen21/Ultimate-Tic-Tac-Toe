INCS := BoardStateMsg.h IMsg.h MarkerMsg.h NextTurnMsg.h PingMsg.h PongMsg.h Serializer.h TurnMsg.h UpdateMsg.h WinStateMsg.h Messaging.h WhoamiMsg.h YouAreMsg.h TurnFailedMsg.h LoginMsg.h LoginReplyMsg.h SignUpMsg.h SignUpReplyMsg.h ErrorMsg.h OpponentNameMsg.h UpdateFinishedMsg.h #ConcurrentQueue.h AsyncSerializer.h
SRCS := BoardStateMsg.cpp MarkerMsg.cpp NextTurnMsg.cpp PingMsg.cpp PongMsg.cpp Serializer.cpp TurnMsg.cpp UpdateMsg.cpp WinStateMsg.cpp WhoamiMsg.cpp YouAreMsg.cpp TurnFailedMsg.cpp LoginMsg.cpp LoginReplyMsg.cpp SignUpMsg.cpp SignUpReplyMsg.cpp ErrorMsg.cpp OpponentNameMsg.cpp UpdateFinishedMsg.cpp #AsyncSerializer.cpp
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
INSTALL_DIR := ../libraries/cpp
INCDIR := $(INSTALL_DIR)/include
DSTINCS := $(patsubst %,$(INCDIR)/%,$(INCS))
LIBDIR := $(INSTALL_DIR)/lib
LIBFILE := $(LIBDIR)/libuttt.a

CXX := g++
CXXFLAGS := -Wall -Werror -g -std=c++11 -pthread -I$(INCDIR)
LDFLAGS  := -L$(LIBDIR) -luttt

all: $(OBJS) install test

$(LIBDIR):
	mkdir $@

install: $(DSTINCS) $(LIBFILE)

$(OBJS): $(DSTINCS)

$(DSTINCS): $(INCDIR)/%.h: ./%.h
	cp -f $*.h $(INCDIR)/

$(LIBFILE): $(LIBDIR) $(OBJS)
	ar -crsv $(LIBFILE) $(OBJS)

test: SerialTest.cpp $(LIBFILE) $(INCS)
	$(CXX) $(CXXFLAGS) -o $@ SerialTest.cpp $(LDFLAGS)

clean:
	-rm -f *~ test *.o
