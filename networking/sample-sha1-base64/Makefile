LIBDIR := ../../libraries/cpp/lib
INCDIR := ../../libraries/cpp/include

CXX := g++
CXXFLAGS := -Wall -I$(INCDIR)
LDFLAGS := -L$(LIBDIR)
LIBS := -l3rdparty

TARGETS := sha1_test base64_test

all: $(TARGETS)

sha1_test: sha1_test.cpp 1000sha1.txt
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ sha1_test.cpp $(LIBS)

base64_test: base64_test.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ base64_test.cpp $(LIBS)

clean:
	-rm -f $(TARGETS) *.o *~
