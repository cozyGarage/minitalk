# 🗣️ Minitalk

A lightweight communication program that demonstrates inter-process communication (IPC) using UNIX signals. This project implements a client-server architecture where messages are transmitted bit-by-bit using only `SIGUSR1` and `SIGUSR2` signals.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Bonus Features](#bonus-features)
- [Technical Details](#technical-details)
- [Compilation Options](#compilation-options)
- [Examples](#examples)
- [Acknowledgments](#acknowledgments)

## 🎯 Overview

Minitalk is a 42 school project that explores the fundamentals of UNIX signals and process communication. The program consists of two parts:

- **Server**: Receives and displays messages from clients
- **Client**: Sends string messages to the server using its Process ID (PID)

The entire communication happens through two UNIX signals (`SIGUSR1` and `SIGUSR2`), with each signal representing a binary bit (0 or 1).

## ✨ Features

### Mandatory Features
- ✅ Server displays its PID on startup
- ✅ Client sends strings to server using server's PID
- ✅ Server displays received messages quickly
- ✅ Multiple clients can communicate with the same server without restart
- ✅ Binary data transmission using only SIGUSR1 and SIGUSR2
- ✅ Error handling for invalid PIDs and signal failures
- ✅ Memory leak free implementation

### Bonus Features
- 🎨 Colored output for better visualization
- 🔄 Server acknowledgment signals back to client
- 📊 Detailed transmission feedback showing each step
- ✅ Support for Unicode characters

## 🔧 How It Works

### Signal-Based Communication

The communication protocol works as follows:

1. **Initialization**: Server starts and displays its PID
2. **Connection**: Client connects to server using the server's PID
3. **Length Transmission**: Client first sends the message length as an integer (32 bits)
4. **Message Transmission**: Client sends each character bit-by-bit
5. **Reception**: Server reconstructs the message from received bits
6. **Display**: Server outputs the complete message

### Bit Encoding

- `SIGUSR1` represents binary `0`
- `SIGUSR2` represents binary `1`

Each character is sent as 8 bits (1 byte), transmitted from most significant bit (MSB) to least significant bit (LSB).

### Example: Sending 'A' (ASCII 65)

```
Binary: 01000001
Signals: SIGUSR1, SIGUSR2, SIGUSR1, SIGUSR1, SIGUSR1, SIGUSR1, SIGUSR1, SIGUSR2
```

## 📦 Prerequisites

- **Operating System**: Linux or macOS (UNIX-based system)
- **Compiler**: GCC or any C compiler with C99 support
- **Required Libraries**: 
  - libft (42 school's standard library) - Note: Must be in `./libft/` directory
  - Standard C libraries (unistd, signal, stdlib)

## 🚀 Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/cozyGarage/minitalk.git
   cd minitalk
   ```

2. **Ensure libft is available**:
   ```bash
   # Make sure libft exists in ./libft/ directory
   # If not, clone or copy your libft implementation
   ```

3. **Compile the project**:
   ```bash
   make
   ```

   This will create two executables:
   - `server`
   - `client`

## 📖 Usage

### Basic Usage

1. **Start the server** in one terminal:
   ```bash
   ./server
   ```
   
   The server will display its PID:
   ```
   server [PID = 12345]
   ```

2. **Send a message** from another terminal:
   ```bash
   ./client <server_PID> "Your message here"
   ```
   
   Example:
   ```bash
   ./client 12345 "Hello, Minitalk!"
   ```

3. **View the result** in the server terminal:
   ```
   Hello, Minitalk!
   ```

### Using Bonus Version

Compile with bonus features:
```bash
make bonus
```

Then run the same way as the basic version. The bonus version provides:
- Colored output showing transmission progress
- Server acknowledgment for each received bit
- Detailed feedback on each character transmission

## 📁 Project Structure

```
minitalk/
├── Makefile              # Build configuration
├── README.md            # This file
│
├── minitalk.h           # Header file for mandatory version
├── server.c             # Server implementation
├── client.c             # Client implementation
├── minitalk.c           # Shared utility functions
├── ft_printf.c          # Custom printf implementation
│
├── minitalk_bonus.h     # Header file for bonus version
├── server_bonus.c       # Server with bonus features
├── client_bonus.c       # Client with bonus features
├── minitalk_bonus.c     # Bonus utility functions
│
├── tester.sh            # Testing script
└── libft/               # 42 standard library (not included in repo)
```

## 🎁 Bonus Features

The bonus implementation includes:

### 1. **Server Acknowledgment**
- Server sends `SIGUSR1` back to client after each bit
- Server sends `SIGUSR2` after complete message reception

### 2. **Visual Feedback**
The bonus client displays colored output:
- 🟡 Yellow: Sending operations
- 🔵 Cyan: Received acknowledgments

Example output:
```bash
$ ./client 12345 "Hi"
Sending length = [2]
Sending message
Sending char [H]
Received ACK from server
Sending char [i]
Received ACK from server
Sending null terminator
Received end of message from server
```

### 3. **Unicode Support**
The bonus version properly handles Unicode and special characters.

## 🔬 Technical Details

### Signal Handling

The program uses `sigaction()` instead of `signal()` for more reliable signal handling:

```c
struct sigaction sa;
sigemptyset(&sa.sa_mask);
sa.sa_sigaction = handler_function;
sa.sa_flags = SA_SIGINFO | SA_RESTART;
sigaction(SIGUSR1, &sa, NULL);
sigaction(SIGUSR2, &sa, NULL);
```

### Protocol Structure

```c
typedef struct s_protocol
{
    int     bits;       // Current bit count
    int     data;       // Accumulated data
    bool    is_ready;   // Ready to receive characters
    char    *str;       // Message buffer
}           t_protocol;
```

### Transmission Flow

1. Client sends message length (32 bits)
2. Server allocates memory based on length
3. Client sends characters one by one (8 bits each)
4. Server reconstructs and stores each character
5. Upon receiving null terminator, server prints message

### Synchronization

- Client uses `pause()` to wait for server acknowledgment
- Server uses `usleep(100)` to stabilize signal processing
- Prevents signal loss and race conditions

## 🛠️ Compilation Options

### Available Make Targets

```bash
make          # Compile mandatory version
make bonus    # Compile bonus version
make clean    # Remove object files (no-op in this project)
make fclean   # Remove executables
make re       # Clean and recompile mandatory
make rebonus  # Clean and recompile bonus
```

### Compilation Flags

```bash
CC = cc
CFLAGS = -Wall -Wextra -Werror
```

The project compiles with strict error checking enabled.

## 💡 Examples

### Example 1: Simple Message
```bash
# Terminal 1
$ ./server
server [PID = 12345]
```

```bash
# Terminal 2
$ ./client 12345 "Hello World"
```

```bash
# Terminal 1 output
Hello World
```

### Example 2: Multiple Messages
The server can receive multiple messages without restarting:

```bash
$ ./client 12345 "First message"
$ ./client 12345 "Second message"
$ ./client 12345 "Third message"
```

Server displays each message as it arrives.

### Example 3: Special Characters
```bash
$ ./client 12345 "Hello! 🌟 Unicode: αβγ"
```

### Example 4: Long Messages
```bash
$ ./client 12345 "Lorem ipsum dolor sit amet, consectetur adipiscing elit..."
```

## 🐛 Error Handling

The program handles various error conditions:

- **Invalid PID**: Client validates server PID before sending
- **Signal Failures**: Both programs check `kill()` return values
- **Memory Allocation**: Server checks `malloc/calloc` results
- **Signal Setup**: Validates `sigaction()` configuration

Example error messages:
```bash
usage: ./client <server PID> <string>
PID is invalid
Error: sending SIGUSR1
Error: memory allocation failed
```

## 📝 Implementation Notes

### Key Considerations

1. **Timing**: Small delays prevent signal loss during rapid transmission
2. **Bit Order**: MSB-first transmission for proper data reconstruction
3. **Memory Management**: Dynamic allocation based on message length
4. **Signal Safety**: Using `SA_RESTART` flag to handle interrupted system calls

### Limitations

- Transmission speed limited by signal processing overhead
- No built-in encryption or message verification
- Single-threaded server handles one message at a time
- Requires valid PID for connection

## 🤝 Acknowledgments

This project is part of the 42 school curriculum, designed to teach:
- UNIX signal handling
- Inter-process communication (IPC)
- Binary data manipulation
- Process synchronization
- Error handling and robustness

## 👤 Author

**thanh-ng** - 42 Vienna

## 📄 License

This project is part of 42 School's curriculum and follows their academic policies.

---

**Note**: This project requires the `libft` library (42's standard library) to be present in the `./libft/` directory. Make sure to include your libft implementation before compiling.

For questions or issues, please refer to the 42 school's project guidelines or contact the author.
