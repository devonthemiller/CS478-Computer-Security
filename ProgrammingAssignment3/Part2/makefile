decrypt: main.o des_cbc.o des.o file.o
	g++ -std=c++11 main.o des_cbc.o des.o file.o -o decrypt -lcrypto

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

des_cbc.o: des_cbc.cpp des_cbc.h
	g++ -std=c++11 -c des_cbc.cpp

des.o: des.cpp des.h des_utils.h
	g++ -std=c++11 -c des.cpp

file.o: file.cpp file.h
	g++ -std=c++11 -c file.cpp

clean:
	rm *.o decrypt
