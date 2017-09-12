INCS := signals.h
OBJS := signals.o
INCDIR := ../include
LIBDIR := ../lib
LIBFILE := $(LIBDIR)/libuttt.a

all: $(OBJS) install

$(LIBDIR):
	mkdir $@

install: $(LIBDIR) $(INCS) $(OBJS)
	cp -f $(INCS) $(INCDIR)
	ar -crsv $(LIBFILE) $(OBJS) 

clean:
	-rm -f *.o *~

