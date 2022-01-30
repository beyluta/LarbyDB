# Welcome to LarbyDB's repository
Make sure you are under a unix system when using the Software.<br>
Developer under g++ version 10.3.0

## Compiling and running the software
Compile it by typing ```g++ -o main -pthread -lpthread main.cpp``` inside the source folder.
To run the software type ```./main```

## Arguments and Parameters
```./main --port <PORT>``` to start the software with a preset port<br>
```./main --tables <TABLES>``` to start the software with a preset number of tables<br>
```./main --safe``` to start the software with a randomly generated authorization key<br>
```./main --unsafe``` to force the software to not require an authorization key on start<br>
```./main --backup``` to start the software with automatic backups<br>
```./main --nobackup``` to start the software without automatic backups<br>
