This project has been created as part of the 42 curriculum by aharder and fyoubi.

Description
ft_irc

ft_irc is a C++ networking project from the 42 curriculum. The goal of this project is to build a functional IRC (Internet Relay Chat) server from scratch, following the IRC protocol and implementing the required features defined by the project subject.

The server is designed to handle multiple clients simultaneously and allows users to communicate through channels using standard IRC commands.

The project focuses on several important concepts:

Network programming with TCP/IP sockets.

Client/server architecture.

Non-blocking I/O and event-driven programming.

Managing multiple simultaneous connections.

Understanding and implementing the IRC protocol.

Parsing and handling IRC commands.

Managing users, channels, permissions, and server state.

Object-oriented programming in C++.

The server can be connected to using an IRC client such as irssi or another compatible IRC client.

Main Features

The server supports the core IRC functionality required by the project, including:

Multiple simultaneous client connections.

User authentication and nickname management.

Private messages between users.

Channel creation and management.

Joining and leaving channels.

Channel messages.

Channel operators and associated privileges.

Channel modes and user modes required by the subject.

IRC command parsing and response handling.

Proper management of client disconnections.

The project is implemented using C++ and relies on the operating system's socket APIs to provide network communication between the server and its clients.

Instructions
Requirements

A Unix-like operating system is recommended.

The project requires a C++ compiler supporting the version of C++ specified by the 42 subject, as well as make.

For example, on Debian/Ubuntu-based systems:

sudo apt update
sudo apt install build-essential

Compilation

Clone the repository and enter the project directory:

git clone <repository-url>
cd ft_irc


Compile the project using:

make


This will build the IRC server executable.

To remove the object files:

make clean


To remove all generated files:

make fclean


To rebuild the project from scratch:

make re

Execution

Start the server by providing the required port and server password:

./ircserv <port> <password>


For example:

./ircserv 6667 password


The server will then listen for incoming TCP connections on the specified port.

Connecting with an IRC Client

Once the server is running, an IRC client can be used to connect to it.

For example, with irssi:

irssi


Then, from inside irssi:

/connect localhost 6667 password


After connecting, clients can use standard IRC commands such as:

/NICK username
/USER username hostname servername :Real Name
/JOIN #channel
/PRIVMSG #channel :Hello everyone!
/PART #channel
/QUIT


The exact commands and available channel modes depend on the implementation required by the project subject.

Resources
IRC Protocol

The project is based on the IRC protocol, so understanding the protocol's message format, commands, replies, and connection model is essential.

RFC 1459 — Internet Relay Chat Protocol

RFC 2812 — Internet Relay Chat: Client Protocol

RFC 2811 — Internet Relay Chat: Channel Management

RFC 2813 — Internet Relay Chat: Server Protocol

These RFCs were used as references for understanding IRC commands, message formatting, numeric replies, channels, and user/server interactions.

Network Programming

The following resources are useful for understanding the networking concepts involved in the project:

socket(2) — Linux socket documentation.

bind(2) — Binding a socket to a local address.

listen(2) — Listening for incoming TCP connections.

accept(2) — Accepting client connections.

send(2) / recv(2) — Sending and receiving data through sockets.

poll(2) — Monitoring multiple file descriptors for I/O events.

C++ References

The project also relies on standard C++ concepts and library features:

C++ reference documentation for the standard library.

C++ documentation concerning classes, inheritance, containers, strings, streams, and exception handling.

POSIX documentation for system calls and file descriptors.

AI Usage

AI tools were used as a supplementary development and learning resource during the project.

They were mainly used for:

Clarifying networking and socket-programming concepts.

Understanding parts of the IRC protocol and RFC documentation.

Helping investigate compiler errors and unexpected runtime behaviour.

Discussing possible approaches to parsing IRC commands and managing client/channel state.

Reviewing code structure and suggesting potential edge cases to test.

Helping with documentation and README organization.

AI was used as an assistance and learning tool rather than as a replacement for understanding or implementing the project. The project's architecture, implementation, debugging, testing, and final code were reviewed and adapted manually.

Project Structure

A typical project structure is organized around the main components of the IRC server:

ft_irc/
├── Makefile
├── include/
│   └── ...
├── src/
│   ├── ...
│   └── ...
└── ...


The exact organization may vary depending on the implementation.

Testing

The server should be tested with multiple IRC clients connected simultaneously in order to verify:

Client connections and disconnections.

Nickname and user registration.

Private messaging.

Channel creation and joining.

Messages sent to channels.

Operator privileges.

Channel modes.

Invalid commands and malformed input.

Multiple clients communicating simultaneously.

Unexpected client disconnections.

Using a real IRC client such as irssi is particularly useful for testing compatibility with standard IRC behaviour.