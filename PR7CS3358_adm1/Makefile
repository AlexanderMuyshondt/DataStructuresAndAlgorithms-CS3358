a7: Assign07Test.o DPQueue.o
	g++ Assign07Test.o DPQueue.o -o a7
Assign07Test.o: Assign07Test.cpp DPQueue.h
	g++ -Wall -ansi -pedantic -c Assign07Test.cpp
DPQueue.o: DPQueue.cpp DPQueue.h
	g++ -Wall -ansi -pedantic -c DPQueue.cpp

clean:
	@rm -rf Assign07Test.o DPQueue.o

cleanall:
	@rm -rf Assign07Test.o DPQueue.o a7
