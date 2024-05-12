# Welcome to LarbyDB's repository

LarbyDB is a fast, lightweight, and easy to use database written in C++. It is designed to communicate seamlessly with IoT devices or web applications. It can be used as a standalone database, cache database, or a temporary database.

## Compatibility

Below is a table of the operating systems that LarbyDB has been tested on.

| **OS**            | **Status**         | _Compiler_  | _Version_ |
| ----------------- | ------------------ | ----------- | --------- |
| Windows >= 11     | :heavy_check_mark: | MINGW64     | 10.3.0    |
| Ubuntu >= 22.04.2 | :heavy_check_mark: | g++         | 10.3.0    |
| MacOS >= 13.4     | :heavy_check_mark: | Apple Clang | 14.0.3    |

<i>Note: Compatibility with other operating systems is plausible but not guaranteed.</i>

## Installation

1. Clone the repo: `git clone https://github.com/beyluta/LarbyDB.git`
2. Navigate to the project's root folder
3. Compile by typing `make unix` for Linux and MacOS. Or `make win` for Windows 10/11
4. To run the software, type `./LarbyDB`

## Settings/Launch Arguments

| **Setting**          | **Description**                                                                 | **Arguments**                                                                                  | **Default value** |
| -------------------- | ------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | :---------------: |
| `manual-config`      | if set to true, asks the user to input<br>parameters upon launching the program | `true`, `false`                                                                                |      `false`      |
| `port`               | sets the port                                                                   | _positive integer_                                                                             |      `8080`       |
| `num-tables`         | sets the number of tables                                                       | _positive integer_                                                                             |        `2`        |
| `allow-backup`       | allow automatic backups                                                         | `true`, `false`                                                                                |      `true`       |
| `backup-interval`    | time interval between backups                                                   | _positive int for seconds_,<br>_positive int+`h` for hours_,<br>_positive int+`m` for minutes_ |       `30m`       |
| `generate-key`       | generate authorization key on launch                                            | `true`, `false`                                                                                |      `true`       |
| `load-backup`        | whether to load from backup on launch                                           | `true`, `false`, `ask`                                                                         |       `ask`       |
| `enable-http`        | whether to use HTTP or TCP                                                      | `true`, `false`, `ask`                                                                         |       `ask`       |

To run with launch arguments add `--` before the parameter name (e.g. `LarbyDB --port 3000`).<br>

# TCP Mode

## Database commands

`AUTH <KEY>` - authorize current device to write and read from the database<br>
`SET <KEY> <TABLE> <EXPIRATION IN SECONDS> <CONTENT>` - set a value<br>
`GET <KEY> <TABLE>` - get a value<br>
`GET ALL <TABLE>` - get all values<br>
`GET ALL <TABLE> <AMOUNT> <SKIP>` - get a specific amount of values<br>
`GET ALL <TABLE> <START-END>` - get values within the specified range<br>
`DEL <KEY> <TABLE>` - delete a value<br>
`DEL <INDEX> <TABLE>` - delete a value by index<br>

## Sending request and receiving responses

When sending a request to the database, the response you might get could be plain text or JSON format depending on how you submitted the data. The following commands will always return a JSON response: `GET ALL`, `GET ALL <START-END>`.<br> If the data you submitted was already JSON-Formatted then you will get a JSON response from the following commands: `GET <KEY> <TABLE>`<br><br>
Here is an example:<br> `SET users 1 0 [{"name":"John"}, {"name":"Titor"}]`<br>`GET users 1`<br>`Response: [{"name":"John"}, {"name":"Titor"}]`<br><br>
Keep in mind that the database sends the following carriage returns that you must strip before using the response: `\r\n\0`

# HTTP Mode (only if --enable-http is true)

Add a value to the database. Send the parameters `table` and `key` in the URL and the data in the body.

```curl
curl -L 'localhost:8080?table=1&key=name' \
-H 'Authorization: Bearer XXXX-XXXXX-XXXXX' \
-H 'Content-Type: application/json' \
-d '{
    "name": "John Doe",
    "age": 30
}'
```

Get a value from the database. Send the parameters `table` and `key` in the URL.

```curl
curl -L 'localhost:8080?key=name&table=1' \
-H 'Content-Type: application/json' \
-H 'Authorization: Bearer XXXX-XXXXX-XXXXX'
```

Get all values from the database. Send the parameter `table` in the URL and `ALL` as the value of the `key` parameter.

```curl
curl -L 'localhost:8080?table=1&key=ALL' \
-H 'Content-Type: application/json' \
-H 'Authorization: Bearer XXXX-XXXXX-XXXXX'
```

Get all values from the database in a certain range. Send the parameters `table`, `from`, `to`, `key` (of which its value must be `ALL`).

```curl
curl -L 'localhost:8080?table=1&key=ALL&from=0&to=10' \
-H 'Content-Type: application/json' \
-H 'Authorization: Bearer XXXX-XXXXX-XXXXX'
```

Delete a value from the database. Send the parameters `table` and `key` in the URL.

```curl
curl -L -X DELETE 'localhost:8080?table=1&key=name' \
-H 'Authorization: Bearer XXXX-XXXXX-XXXXX' \
```
