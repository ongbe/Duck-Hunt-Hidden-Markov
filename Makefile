both: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt server load enviroments/SouthEmissions.in < bin/player2server | ./bin/duckhunt verbose > bin/player2server

bin/duckhunt: src/player.cpp
	g++ src/*.cpp -Wall -std=c++0x -o bin/duckhunt

bin/player2server bin/server2player:
	mkfifo bin/player2server bin/server2player

server: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt verbose server < bin/player2server > bin/server2player

client: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt verbose > bin/player2server < bin/server2player

ParadiseEmissions: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt server load enviroments/ParadiseEmissions.in < bin/player2server | ./bin/duckhunt verbose > bin/player2server
