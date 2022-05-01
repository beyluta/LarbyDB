all:
	g++ -o LarbyDB -pthread -lpthread src/main.cpp
clear:
	rm LarbyDB