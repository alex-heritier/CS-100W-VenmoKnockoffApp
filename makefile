FLAGS = -g -Wall -std=c++11 -stdlib=libc++
LFLAGS = $(FLAGS) -lncurses
CFLAGS = $(FLAGS) -c

build/main: lib/MainClass.o lib/NonexistentCommandException.o lib/VenmoKnockoffApp.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o
	g++ $(LFLAGS) lib/MainClass.o lib/NonexistentCommandException.o lib/VenmoKnockoffApp.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o -o build/main

lib/MainClass.o: src/MainClass.cpp src/NonexistentCommandException.hpp src/VenmoKnockoffApp.hpp
	g++ $(CFLAGS) src/MainClass.cpp -o lib/MainClass.o

lib/NonexistentCommandException.o: src/NonexistentCommandException.hpp src/NonexistentCommandException.cpp
	g++ $(CFLAGS) src/NonexistentCommandException.cpp -o lib/NonexistentCommandException.o

lib/VenmoKnockoffApp.o: src/VenmoKnockoffApp.hpp src/VenmoKnockoffApp.cpp src/User.hpp
	g++ $(CFLAGS) src/VenmoKnockoffApp.cpp -o lib/VenmoKnockoffApp.o

lib/User.o: src/User.hpp src/User.cpp src/helper_funs.hpp
	g++ $(CFLAGS) src/User.cpp -o lib/User.o

lib/NotEnoughFundsException.o: src/NotEnoughFundsException.hpp src/NotEnoughFundsException.cpp
	g++ $(CFLAGS) src/NotEnoughFundsException.cpp -o lib/NotEnoughFundsException.o

lib/helper_funs.o: src/helper_funs.hpp src/helper_funs.cpp
	g++ $(CFLAGS) src/helper_funs.cpp -o lib/helper_funs.o

clean:
	rm -f build/*
	rm -f lib/*

run: build/main
	./build/main
