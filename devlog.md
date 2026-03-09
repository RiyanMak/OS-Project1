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