# Devlog

## 2026-03-06 15:00

### Initial Thoughts

This is the start of my project. Based on the specifications, I need to build a logger, encryption and decryption program. 


### Overall Plan

I'm thinking of using the C programming language that will support forks and pipes. 
Initially I am thinking of making a makefile
Then working on implementing the logger
Then implementing the encryption program
Then the decryption program

After all of that comes the testing phase where i will write some test cases to 
determine if the code is running as intended

### Open Questions

- I'm not totally sure on how exactly i will be setting up the encryption and decryption. I dont understand Vigenère cypher 
  and how to exactly work with it 

---

## 2026-03-09 1:50

### Initial Thoughts

I need to build a logger function which can open a text file and write the log message in this format 
YYYY-MM-DD HH:MM [ACTION] MESSAGE

### Overall Plan

Will have to use some sort of time library in c and then will also have to use a library that will allow us 
I/O operations on a file

### Open Questions

- Not sure how to correctly implement the logger program for encryption and decryption might need to refactor later

---


## 2026-03-09 16:55

### Initial Thoughts

Have to handle errors when file doesn't exist and select the mode for the file

### Overall Plan

Will have to use some sort of time library in c and then will also have to use a library that will allow us 
I/O operations on a file. Might need some libraries in C to return errors for file not existing as well

### Open Questions

- how to exactly take the command line arguments are write it into the file
---

## 2026-03-11 16:05

### Initial Thoughts

I have to read the argument passed using standard input will need to implement either a buffer or some array to hold that value

### Overall Plan

Will have to implement an array to hold the value of the ACTION and Message. Might need to implement a buffer to hold the values temporarily.

### Open Questions

- I dont know exactly on how this will be done and if this is the correct approach i am going to attempt it

---


## 2026-03-13 12:00

### Initial Thoughts

I have completed the logger program and now onto the encryption
### Overall Plan

I am going to manually trace on paper given a test string
like HELLO and try to write psuedocode for it

### Open Questions

- There are many ways to implement Vingere Cypher just have to find a method that works for me

---

## 2026-03-13 13:30

### Initial Thoughts

Encryption is done and working. Tested it with HELLO and the key HELLO and got OIWWC which matches what the spec shows so that is good. Now i have to work on the driver program which is the hardest part since i have to fork two processes and connect them with pipes.

### Overall Plan

Will have to use fork twice, once for the logger and once for the encryption program.
Then use pipe and dup2 to connect their stdin and stdout to the driver.
Then implement the command loop with password, encrypt, decrypt, history, and quit.
Also have to make sure passwords are never logged and that input is validated to be letters only.

### Open Questions

- Not totally sure how to manage all the pipe ends correctly, going to have to be careful about what to close in each child vs the parent

---

## 2026-03-13 15:30

### Initial Thoughts

Driver is done and tested. The hardest part was keeping track of which pipe ends to close in each process. Also ran into a bug where i was sending "QUIT Driver exiting." to the logger but since QUIT is what tells the logger to stop, it was never actually writing the exit message to the log file. Had to fix it so the exit message gets logged first and then QUIT gets sent separately. Also noticed the history command was not being logged so i added that in.

### Overall Plan

Just have to write the README now and then zip everything up to submit.

### Open Questions

- Nothing left, just wrapping up

---