# Communication System

## Overview
This project provides a communication system that allows multiple instances to send and receive messages over specified ports. Each instance operates in a separate terminal window and offers a menu-driven interface to manage messages.

## Files and Directories

- `brain.c`: Handles communication for each system instance and provides a menu for sending and receiving messages.
- `launch.c`: Reads system IDs and ports from a configuration file and launches instances of `brain.c` in separate terminal windows.
- `ur.c`: Receives messages on a specified port.
- `us.c`: Sends messages to a specified port.
- `sys_info.txt`: Configuration file containing system IDs and ports.

## Compilation

To compile the programs, use the provided Makefile.

### Using the Makefile

1. Compile Programs. Run the following command to compile all programs:
    -> make
   This will generate the executables: `brain`, `launch`, `ur`, and `us`.

2. Clean Up . To remove all compiled binaries and build artifacts, use:
   -> make clean
   This command deletes the executables and other build files.

## Configuration

1. Prepare the Configuration File
   Create a file named `sys_info.txt` with the following format:
   sys1 2001
   sys2 2002
   sys3 2003
   Each line should contain a system ID followed by a port number.

## Usage
### Launch Instances
To start the communication system, run:

./launch sys_info.txt

This command reads the `sys_info.txt` file and launches the `brain` program in separate terminal windows for each system listed in the file.

### Interact with Menu

In each terminal window, you will see a menu like this:

===== Communication Menu =====
System <number> (Port: <port>)
Select what to do:
1) View Sent Messages
2) View Received Messages
3) Send Messages
4) EXIT
Enter your choice:

1. View Sent Messages :- Shows all messages sent from the current system. Logs are saved in `sentmsg_<port>.txt`.
2. View Received Messages :- Shows all messages received by the current system. Logs are saved in `recvMessage_<port>.txt`.
3. Send Messages :- Allows sending a message to another system.
4. EXIT :- Exits the program and deletes the message logs for the current system.

### Sending Messages

To send a message, choose the "Send Messages" option from the menu and provide the recipient system ID and message text. The `us` program will handle the sending.

### Receiving Messages

The `ur` program listens on the specified port for incoming messages. Received messages are logged in `recvMessage_<port>.txt`.

## Message Logs

- Sent Messages :- Stored in `sentmsg_<port>.txt`.
- **Received Messages :- Stored in `recvMessage_<port>.txt`.

These logs are created when the system starts and deleted when the system exits.


- Program not running :-Ensure all source files are compiled correctly and the `sys_info.txt` file is properly formatted.
- Communication issues :- Verify that the ports specified in `sys_info.txt` are available and not blocked by a firewall.

