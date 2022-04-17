all:
	g++ -o LarbyDB -pthread -lpthread jsonconverter.cpp socket.cpp timer.cpp file.cpp backuphandler.cpp controller.cpp hashtable.cpp src/main.cpp
clear:
	rm LarbyDB