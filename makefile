all:
	g++ -o LarbyDB -pthread -lpthread -std=c++17 src/jsonconverter.cpp src/socket.cpp src/timer.cpp src/file.cpp src/backuphandler.cpp src/controller.cpp src/hashtable.cpp src/logsys.cpp src/linknode.cpp src/main.cpp
clear:
	rm LarbyDB
start:
	clear
	make all
	./LarbyDB