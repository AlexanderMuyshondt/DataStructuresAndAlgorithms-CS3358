llcp: llcpImp.o Assign05P1.o
	g++ llcpImp.o Assign05P1.o -o a5p1
llcpImp.o: llcpImp.cpp llcpInt.h
	g++ -Wall -ansi -pedantic -c llcpImp.cpp
Assign05P1.o: Assign05P1.cpp llcpInt.h
	g++ -Wall -ansi -pedantic -c Assign05P1.cpp

go:
	./a5p1
gogo:
	./a5p1 > a5p1test.out

clean:
	@rm -rf llcpImp.o Assign05P1.o
cleanall:
	@rm -rf llcpImp.o Assign05P1.o a5p1
