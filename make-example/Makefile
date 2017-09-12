all: hello goodbye hanginthere

# target of hello
hello: hello.cpp
	g++ -Wall -Werror -o hello hello.cpp
# commands above this line
# that is a literal TAB character at the front of the command


goodbye: goodbye.cpp
	g++ -Wall -Werror -o goodbye goodbye.cpp
# commands above this line
# that is a literal TAB character at the front of the command


# linker stage explicitly
hanginthere: hanginthere.o hangin.o
	g++ -Wall -Werror -o hanginthere hanginthere.o hangin.o

hanginthere.o: hanginthere.cpp hangin.h
	g++ -Wall -Werror -c hanginthere.cpp

hangin.o: hangin.cpp hangin.h
	g++ -Wall -Werror -c hangin.cpp

clean:
	-rm -f *~ hello goodbye hanginthere *.o

spotless: clean
	-rm -f hello

notsplotless: hello
	echo "HELLO"
