# Welcome to LarbyDB's repository
Make sure you are under a unix system when using the Software.<br>
Developed under `g++ version 10.3.0`

## Installation

1. Clone the repo: `git clone https://github.com/beyluta/LarbyDB.git`
2. Compile by typing `make` while inside the project's folder.<br>
Alternatively, you can compile it with g++: `g++ -o LarbyDB -pthread -lpthread src/main.cpp`
3. To run the software, type `./LarbyDB`

## Launch Arguments
`--port <PORT>` - set the port<br>
`--tables <TABLES>` - set the number of tables<br>
`--safe` - start with a randomly generated authorization key<br>
`--unsafe` - force to not require an authorization key on start<br>
`--backup` - start with automatic backups<br>
`--nobackup` - start without automatic backups<br>
`--autoload` - make it load the backup automatically<br>

## Database commands
`SET <KEY> <TABLE> <EXPIRATION IN SECONDS> <CONTENT>` - set a value<br>
`GET <KEY> <TABLE>` - get a value<br>
`GET ALL <TABLE>` - get all values<br>
`DEL <KEY> <TABLE>` - delete a value<br>
`DEL <INDEX> <TABLE>` - delete a value by index<br>
