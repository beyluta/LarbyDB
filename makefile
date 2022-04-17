all:
	g++ -o LarbyDB -pthread -lpthread jsonconverter.cpp socket.cpp timer.cpp src/main.cpp
clear:
	rm LarbyDB