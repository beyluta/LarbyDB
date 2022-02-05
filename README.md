# Welcome to LarbyDB's repository
Make sure you are under a unix system when using the Software.<br>
Developed under `g++ version 10.3.0`

## Compiling manually
Compile it by typing `g++ -o main -pthread -lpthread main.cpp` inside the source folder then to run the software type `./main`<br>
## Compiling with ```make```
In the `/src` folder type `make` in the terminal then `./main` to run the software.

## Arguments and Parameters
`./main --port <PORT>` to start the software with a preset port<br>
`./main --tables <TABLES>` to start the software with a preset number of tables<br>
`./main --safe` to start the software with a randomly generated authorization key<br>
`./main --unsafe` to force the software to not require an authorization key on start<br>
`./main --backup` to start the software with automatic backups<br>
`./main --nobackup` to start the software without automatic backups<br>
