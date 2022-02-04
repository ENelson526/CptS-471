main: main.o alignments.o
	g++ -g main.o alignments.o

main.o: main.cpp alignments.hpp
	g++ -g -c main.cpp

alignments.o: alignments.cpp alignments.hpp
	g++ -g -c alignments.cpp

clean:
	-rm *.o