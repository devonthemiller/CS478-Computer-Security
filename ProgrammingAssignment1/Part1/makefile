substitutioncracker: main.o functions.o
	g++ main.o functions.o -o substitutioncracker

main.o: main.cpp
	g++ -c main.cpp

functions.o: functions.cpp functions.h
	g++ -c functions.cpp

clean:
	rm *.o substitutioncracker
