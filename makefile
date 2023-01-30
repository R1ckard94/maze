program: main.o
	g++ main.o mazeFunctions.o -o maze

main.o: main.cpp
	g++ -std=c++11 -c main.cpp mazeFunctions.cpp

clean:
	rm *.o maze
