both: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt server load enviroments/SouthEmissions.in < bin/player2server | ./bin/duckhunt verbose > bin/player2server

bin/duckhunt: src/player.cpp src/BaumWelch.hpp src/Matrix.hpp src/HiddenMarkovModel.hpp src/helpers.hpp
	g++ src/player.cpp -g lib/*.cpp -Wall -std=c++0x -o bin/duckhunt

bin/server:
	g++ lib/*.cpp src/player.cpp -g -std=c++0x -o bin/server

clean:
	rm -r bin/*

bin/player2server bin/server2player:
	mkfifo bin/player2server bin/server2player

server: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt verbose load enviroments/NorthEmissions.in server < bin/player2server > bin/server2player

client: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt verbose > bin/player2server < bin/server2player

ParadiseEmissions SouthEmissions EastEmissions NorthEmissions WestEmissions: bin/player2server bin/server2player bin/duckhunt
	./bin/duckhunt server load enviroments/$@.in < bin/player2server | ./bin/duckhunt verbose > bin/player2server




# HMM

nextemissiontest: src/nextemission.cpp src/Matrix.hpp src/HiddenMarkovModel.hpp
	g++ -g src/nextemission.cpp -Wall -std=c++0x -o bin/nextemission
	bash -c "cat enviroments/nextemissiontest | ./bin/nextemission | diff <(echo 1 3 0.3 0.6 0.1) -"

probabilityofemissionsequencetest: src/emissionprob.cpp src/Matrix.hpp src/HiddenMarkovModel.hpp
	g++ -g src/emissionprob.cpp -Wall -std=c++0x -o bin/probabilityofemissionsequencetest
	bash -c "cat enviroments/probabilityofemissionsequencetest | ./bin/probabilityofemissionsequencetest | diff <(echo 0.0902755) -"

mostlikly: src/mostliklyseq.cpp src/Matrix.hpp src/HiddenMarkovModel.hpp
	g++ src/mostliklyseq.cpp -Wall -g -std=c++0x -o bin/mostliklyseq
	bash -c "cat enviroments/mostliklytest | ./bin/mostliklyseq | diff <(echo 0 1 2 1) -"

estimatemodel: src/estimatemodel.cpp src/Matrix.hpp src/HiddenMarkovModel.hpp src/BaumWelch.hpp
	g++ src/estimatemodel.cpp -Wall -g -std=c++0x -o bin/estimatemodel
	bash -c "cat enviroments/estimatemodeltest | ./bin/estimatemodel | diff <(cat enviroments/esitmatemodel.out) -"