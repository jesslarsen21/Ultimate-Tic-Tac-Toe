INCS := base64.h
OBJS := base64.o
INCDIR := ../include
LIBDIR := ../lib
LIBFILE := $(LIBDIR)/lib3rdparty.a

all: $(OBJS) install

$(LIBDIR):
	mkdir $@

install: $(LIBDIR) $(INCS) $(OBJS)
	cp -f $(INCS) $(INCDIR)
	ar -crsv $(LIBFILE) $(OBJS) 

clean:
	-rm -f *.o *~

