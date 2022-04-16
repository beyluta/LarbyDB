all:
	g++ -o LarbyDB -pthread -lpthread jsonconverter.cpp src/main.cpp
clear:
	rm LarbyDB