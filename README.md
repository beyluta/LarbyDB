# Welcome to LarbyDB's repository
Make sure you are under a unix system when using the Software.<br>
Developed under `g++ version 10.3.0`

## Installation

1. Clone the repo: `git clone https://github.com/beyluta/LarbyDB.git`
2. Compile by typing `make` while inside the project's folder.<br>
3. To run the software, type `./LarbyDB`

## Settings/Launch Arguments

| **Setting**       | **Description**                      | **Arguments**      | **Default value** |
|-------------------|--------------------------------------|--------------------|:-----------------:|
| `manual_config`   | if set to true, asks the user to input<br>parameters upon launching the program | `true`, `false`| `false` |
| `port`            | sets the port                        | *positive integer* | `8080`            |
| `num_tables`      | sets the number of tables            | *positive integer* | `2`               |
| `allow_backup`    | allow automatic backups              | `true`, `false`    | `true`            |
| `backup_interval` | time interval between backups |*positive int for seconds*,<br>*positive int+`h` for hours*,<br>*positive int+`m` for minutes*   |`30m`|
| `generate_key`    | generate authorization key on launch | `true`, `false`    | `true`            |
| `load_backup`     | whether to load from backup on launch| `true`, `false`, `ask`| `ask`          |

To run with launch arguments add `--` before the parameter name (e.g. `LarbyDB --port 3000`).<br>

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
