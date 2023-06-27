FLAGS += -std=c++20 -I ./src -Wall

all: server
	-

server: main.cpp src/socket.cpp src/server.cpp src/msg_parser.cpp
	g++ $(FLAGS) main.cpp src/socket.cpp src/server.cpp src/msg_parser.cpp -o server

clean:
	rm server