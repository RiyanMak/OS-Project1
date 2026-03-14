# CS4348 Project 1 - Riyan Maknojia

## Files

- `logger.c` / `logger.h` - The logger program. Reads log messages from stdin and writes them to a log file with timestamps.
- `encryption.c` / `encryption.h` - The encryption program. Handles PASSKEY, ENCRYPT, and DECRYPT commands using a Vigenere cipher.
- `driver.c` - The driver program. Launches the logger and encryption programs as child processes and connects them with pipes. Handles the interactive menu for the user.
- `main.c` - Entry point for the logger executable.
- `Makefile` - Builds all three programs.

## How to Compile

Run the following from the project directory:

```
make
```

This will produce three executables: `logger`, `encryption`, and `driver`.

## How to Run

```
./driver <logfile>
```

Example:

```
./driver logfile.txt
```

The driver will launch the logger and encryption programs automatically. You will be shown a menu with the following commands: `password`, `encrypt`, `decrypt`, `history`, `quit`.

## Notes for TA

- The driver uses fork, pipe, and dup2 as required.
- Passwords are never written to the log file.
- All input is converted to uppercase before being sent to the encryption program.
- Input is validated to contain letters only.
