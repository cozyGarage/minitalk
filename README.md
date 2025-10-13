# What does sigempty set does?

`sigemptyset` is a C library function that initializes a signal set to be empty, meaning it contains no signals.

### Usage

- **Purpose:** Prepares a `sigset_t` structure so you can add signals to it later.
- **Common Context:** Used when setting up signal handlers (like with `sigaction`), to specify which signals should be blocked during handler execution.

### Example

```c
sigset_t set;
sigemptyset(&set); // 'set' now contains no signals
```

### Gotcha

If you forget to call `sigemptyset` before adding signals (with `sigaddset`), the set may contain garbage values, leading to unpredictable behavior.

### In Your Code

```c
sigemptyset(&sa.sa_mask);
```
This line ensures that no signals are blocked while your signal handler runs (unless you add some later).

#

Here’s how the client and server communicate and send bits using UNIX signals:

1. **Client Side:**
   - The client takes the server’s PID and the string to send.
   - For each character in the string, the client loops through its bits (from most significant to least).
   - For each bit:
     - If the bit is 0, the client sends SIGUSR1 to the server using `kill(server_pid, SIGUSR1)`.
     - If the bit is 1, the client sends SIGUSR2 to the server using `kill(server_pid, SIGUSR2)`.
   - The client repeats this for every character, including a terminating `'\0'` to signal the end.

2. **Server Side:**
   - The server sets up signal handlers for SIGUSR1 and SIGUSR2.
   - Each time it receives a signal:
     - If SIGUSR1, it appends a 0 bit to the current character.
     - If SIGUSR2, it appends a 1 bit.
   - After receiving 8 signals (bits), the server has a complete character and prints it.
   - The process repeats for each character sent by the client.

**Summary:**  
The client encodes each character as a sequence of 8 signals (SIGUSR1 for 0, SIGUSR2 for 1). The server decodes these signals back into characters and prints them. This is a simple bitwise protocol using only UNIX signals for communication.

# What is SA_RESTART
`SA_RESTART` is a flag used with the `struct sigaction` in signal handling on Unix-like systems. When set, it causes certain system calls (like `read`, `write`, `pause`, etc.) that are interrupted by signals to automatically restart instead of failing with an `EINTR` error. This helps make signal handling more robust and prevents unexpected interruptions in your program’s flow.