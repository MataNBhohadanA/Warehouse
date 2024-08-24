all: bin/main


bin/main: bin/main.o bin/WareHouse.o bin/Action.o bin/Customer.o bin/Order.o bin/Volunteer.o
	g++ -o bin/main bin/main.o bin/WareHouse.o bin/Action.o bin/Customer.o bin/Order.o bin/Volunteer.o

bin/main.o:src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/Volunteer.o:src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp

bin/Order.o:src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp

bin/Customer.o:src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Action.o:src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/WareHouse.o:src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

clean:
	rm -f bin/*

