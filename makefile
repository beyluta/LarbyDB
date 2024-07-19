unix:
	g++ -o LarbyDB -pthread -lpthread -std=c++17 src/sort.cpp src/jsonconverter.cpp src/socket.cpp src/timer.cpp src/file.cpp src/backuphandler.cpp src/controller.cpp src/hashtable.cpp src/logsys.cpp src/linknode.cpp src/main.cpp -Iinclude
win:
	g++ -o LarbyDB -pthread -lpthread -lShell32 -std=c++17 src/sort.cpp src/jsonconverter.cpp src/socket.cpp src/timer.cpp src/file.cpp src/backuphandler.cpp src/controller.cpp src/hashtable.cpp src/logsys.cpp src/linknode.cpp src/main.cpp -Iinclude -lwsock32 -lws2_32