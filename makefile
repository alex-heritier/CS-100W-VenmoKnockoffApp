FLAGS = -g -Wall -std=c++11 
ifeq ($(shell uname),Darwin)
	FLAGS += -stdlib=libc++ -I/Users/Alex/School/17th\ Grade/sem1/CS\ 144/project/lib/boost_1_62_0
	FLAGS += -L/usr/local/lib
endif
LFLAGS = $(FLAGS)
CFLAGS = $(FLAGS) -c

# VKA_User client
build/main: lib/VKA_User.o lib/NonexistentCommandException.o lib/VKAClient.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o lib/ServerConnection.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o
	g++ $(LFLAGS) lib/VKA_User.o lib/NonexistentCommandException.o lib/VKAClient.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o lib/ServerConnection.o lib/Serializable.o lib/UserData.o lib/PayData.o  lib/AddFundsData.o -o build/main 

lib/VKA_User.o: src/VKA_User.cpp src/NonexistentCommandException.hpp src/VKAClient.hpp
	g++ $(CFLAGS) src/VKA_User.cpp -o lib/VKA_User.o

lib/NonexistentCommandException.o: src/NonexistentCommandException.cpp src/NonexistentCommandException.hpp
	g++ $(CFLAGS) src/NonexistentCommandException.cpp -o lib/NonexistentCommandException.o

lib/VKAClient.o: src/VKAClient.cpp src/VKAClient.hpp src/User.hpp src/ServerConnection.hpp src/CommandData.hpp src/UserData.hpp src/PayData.hpp src/AddFundsData.hpp
	g++ $(CFLAGS) src/VKAClient.cpp -o lib/VKAClient.o

lib/User.o: src/User.cpp src/User.hpp src/helper_funs.hpp
	g++ $(CFLAGS) src/User.cpp -o lib/User.o
	
lib/FundSource.o: src/FundSource.cpp src/FundSource.hpp
	g++ $(CFLAGS) src/FundSource.cpp -o lib/FundSource.o
	
lib/Bank.o: src/Bank.cpp src/Bank.hpp
	g++ $(CFLAGS) src/Bank.cpp -o lib/Bank.o
	
lib/Card.o: src/Card.cpp src/Card.hpp
	g++ $(CFLAGS) src/Card.cpp -o lib/Card.o

lib/NotEnoughFundsException.o: src/NotEnoughFundsException.cpp src/NotEnoughFundsException.hpp
	g++ $(CFLAGS) src/NotEnoughFundsException.cpp -o lib/NotEnoughFundsException.o

lib/helper_funs.o: src/helper_funs.cpp src/helper_funs.hpp
	g++ $(CFLAGS) src/helper_funs.cpp -o lib/helper_funs.o

lib/ServerConnection.o: src/ServerConnection.cpp src/ServerConnection.hpp
	g++ $(CFLAGS) src/ServerConnection.cpp -o lib/ServerConnection.o

lib/Serializable.o: src/Serializable.cpp src/Serializable.hpp
	g++ $(CFLAGS) src/Serializable.cpp -o lib/Serializable.o

lib/UserData.o: src/UserData.cpp src/UserData.hpp src/Serializable.hpp src/CommandData.hpp
	g++ $(CFLAGS) src/UserData.cpp -o lib/UserData.o

lib/PayData.o: src/PayData.cpp src/PayData.hpp src/Serializable.hpp src/CommandData.hpp
	g++ $(CFLAGS) src/PayData.cpp -o lib/PayData.o

lib/AddFundsData.o: src/AddFundsData.cpp src/AddFundsData.hpp src/Serializable.hpp src/CommandData.hpp
	g++ $(CFLAGS) src/AddFundsData.cpp -o lib/AddFundsData.o

# VKALocalServer server
build/server: lib/VKALocalServer.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o lib/ServerConnection.o lib/User.o lib/FundSource.o lib/Bank.o lib/Card.o lib/NotEnoughFundsException.o lib/helper_funs.o
	g++ $(LFLAGS) lib/VKALocalServer.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o lib/ServerConnection.o lib/User.o lib/FundSource.o lib/Bank.o lib/Card.o lib/NotEnoughFundsException.o lib/helper_funs.o -o build/server -lboost_serialization

lib/VKALocalServer.o: src/VKALocalServer.cpp src/CommandData.hpp src/Serializable.hpp src/UserData.hpp src/PayData.hpp src/AddFundsData.hpp src/ServerConnection.hpp src/User.hpp src/NotEnoughFundsException.hpp src/helper_funs.hpp
	g++ $(CFLAGS) src/VKALocalServer.cpp -o lib/VKALocalServer.o

# VKA_AI client
build/ai: lib/VKA_AI.o lib/VKAClient.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o lib/ServerConnection.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o
	g++ $(LFLAGS) lib/VKA_AI.o lib/VKAClient.o lib/User.o lib/NotEnoughFundsException.o lib/helper_funs.o lib/ServerConnection.o lib/Serializable.o lib/UserData.o lib/PayData.o lib/AddFundsData.o -o build/ai

lib/VKA_AI.o: src/VKA_AI.cpp src/VKAClient.hpp
	g++ $(CFLAGS) src/VKA_AI.cpp -o lib/VKA_AI.o

# General stuff
clean:
	rm -f build/*
	rm -f lib/*

run: build/main
	./build/main

server: build/server
	./build/server

ai: build/ai
	./build/ai
