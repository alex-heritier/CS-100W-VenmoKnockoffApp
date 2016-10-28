FLAGS = -g -Wall -std=c++11 -stdlib=libc++
LFLAGS = $(FLAGS)
CFLAGS = $(FLAGS) -c

# MainClass clients
build/main: lib/MainClass.o lib/NonexistentCommandException.o lib/VenmoKnockoffApp.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o lib/ServerConnection.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o
	g++ $(LFLAGS) lib/MainClass.o lib/NonexistentCommandException.o lib/VenmoKnockoffApp.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o lib/ServerConnection.o lib/Serializable.o lib/UserData.o lib/PayData.o  lib/AddFundsData.o -o build/main

lib/MainClass.o: src/MainClass.cpp src/NonexistentCommandException.hpp src/VenmoKnockoffApp.hpp
	g++ $(CFLAGS) src/MainClass.cpp -o lib/MainClass.o

lib/NonexistentCommandException.o: src/NonexistentCommandException.cpp src/NonexistentCommandException.hpp
	g++ $(CFLAGS) src/NonexistentCommandException.cpp -o lib/NonexistentCommandException.o

lib/VenmoKnockoffApp.o: src/VenmoKnockoffApp.cpp src/VenmoKnockoffApp.hpp src/User.hpp src/ServerConnection.hpp src/CommandData.hpp src/UserData.hpp src/PayData.hpp src/AddFundsData.hpp
	g++ $(CFLAGS) src/VenmoKnockoffApp.cpp -o lib/VenmoKnockoffApp.o

lib/User.o: src/User.cpp src/User.hpp src/helper_funs.hpp
	g++ $(CFLAGS) src/User.cpp -o lib/User.o

lib/NotEnoughFundsException.o: src/NotEnoughFundsException.cpp src/NotEnoughFundsException.hpp
	g++ $(CFLAGS) src/NotEnoughFundsException.cpp -o lib/NotEnoughFundsException.o

lib/helper_funs.o: src/helper_funs.cpp src/helper_funs.hpp
	g++ $(CFLAGS) src/helper_funs.cpp -o lib/helper_funs.o

lib/ServerConnection.o: src/ServerConnection.cpp src/ServerConnection.hpp
	g++ $(CFLAGS) src/ServerConnection.cpp -o lib/ServerConnection.o

lib/Serializable.o: src/Serializable.cpp src/Serializable.hpp
	g++ $(CFLAGS) src/Serializable.cpp -o lib/Serializable.o

lib/UserData.o: src/UserData.cpp src/UserData.hpp src/Serializable.hpp
	g++ $(CFLAGS) src/UserData.cpp -o lib/UserData.o

lib/PayData.o: src/PayData.cpp src/PayData.hpp src/Serializable.hpp
	g++ $(CFLAGS) src/PayData.cpp -o lib/PayData.o

lib/AddFundsData.o: src/AddFundsData.cpp src/AddFundsData.hpp src/Serializable.hpp
	g++ $(CFLAGS) src/AddFundsData.cpp -o lib/AddFundsData.o

# VKALocalServer server
build/server: lib/VKALocalServer.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o lib/ServerConnection.o
	g++ $(LFLAGS) lib/VKALocalServer.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o lib/ServerConnection.o -o build/server

lib/VKALocalServer.o: src/VKALocalServer.cpp src/CommandData.hpp src/Serializable.hpp src/UserData.hpp src/PayData.hpp src/AddFundsData.hpp src/ServerConnection.hpp
	g++ $(CFLAGS) src/VKALocalServer.cpp -o lib/VKALocalServer.o

# General stuff
clean:
	rm -f build/*
	rm -f lib/*

run: build/main
	./build/main

server: build/server
	./build/server
