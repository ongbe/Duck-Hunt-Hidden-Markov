both: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt server load enviroments/SouthEmissions.in < bin/player2server | ./bin/duckhunt verbose > bin/player2server

bin/duckhunt: src/player.cpp
	g++ src/player.cpp lib/*.cpp -Wall -std=c++0x -o bin/duckhunt

bin/player2server bin/server2player:
	mkfifo bin/player2server bin/server2player

server: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt verbose server < bin/player2server > bin/server2player

client: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt verbose > bin/player2server < bin/server2player

ParadiseEmissions: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt server load enviroments/ParadiseEmissions.in < bin/player2server | ./bin/duckhunt verbose > bin/player2server

test: nextemissiontest

nextemissiontest: src/nextemission.cpp src/Matrix.hpp src/HiddenMarkovModel.hpp
	g++ src/nextemission.cpp -Wall -std=c++0x -o bin/nextemission
	bash -c "cat enviroments/nextemissiontest | ./bin/nextemission | diff <(echo 1 3 0.3 0.6 0.1) -"

clean:
	rm bin/*

kattisnextemission:
	open "https://kth.kattis.com/submissions/`kattis -f -p kth:ai:hmm1 src/nextemission.cpp src/HiddenMarkovModel.hpp src/Matrix.hpp | pcregrep -o1 'Submission ID: (\d+)\.'`"