all: debugging_demo

debugging_demo: debugging_demo.cpp
	g++ -Wall -Werror -g -o debugging_demo debugging_demo.cpp

debug:
	gdb ./debugging_demo


clean:
	-rm -f debugging_demo *~ *.o
