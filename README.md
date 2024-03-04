# Operating Systems Course: Assignment 3 - Proactor, Multi-Threads  using async I/O

## Overview

This assignment is a crucial part of the Operating Systems Course, aiming to enhance skills in multithreaded programming and the application of the proactor design pattern.
We are tasked with developing a chat server and client but under the constraints of a multithreaded environment without using asynchronous I/O.
The project unfolds in three sections: 
- creating a basic chat functionality in a multithreaded setup.
- implementing a general proactor pattern.
- finally integrating the proactor into the chat server.
  
This progression is designed to deepen understanding of handling multiple client connections, thread management, and event-driven programming,
culminating in a comprehensive exploration of advanced networking concepts within a Unix/Linux environment.

## Objective

The primary goal is to construct a multithreaded chat server and client that navigate the complexities of multiple annoyances without resorting to multi-process or asynchronous I/O solutions. This includes:

- **Section A:** Building a chat system that operates in a busy environment, using threads to manage client-server interactions.
- **Section B:** Developing a general proactor pattern utilizing the chat application from Section A, emphasizing dynamic thread creation for handling connections.
- **Section C:** Applying the proactor pattern to reimagine the chat server from Section A, demonstrating the practical benefits of this model in real-world applications.

## Getting Started
Begin with a thorough review of Beej's Guide to Network Programming to solidify your foundation in network programming concepts. This knowledge is crucial for tackling the multithreaded chat server and client implementation. Following this, engage with each section sequentially, ensuring a strong grasp of thread management and the proactor pattern before attempting to integrate these components. The assignment requires a comprehensive understanding of socket programming, thread synchronization, and event-driven architecture, all within a Unix/Linux development environment.
- [Beej's Guide to Network Programming - Server Example](https://beej.us/guide/bgnet/examples/server.c)
- [Beej's Guide to Network Programming - Client Example](https://beej.us/guide/bgnet/examples/client.c)

## Project Structure

### Server

- **Multithreading for Concurrent Connections:** The server is designed to support multiple simultaneous connections in a multithreaded environment, avoiding the use of multi-process synchronization or asynchronous I/O. Each connected client is handled by a dedicated thread that listens to communication from that client, ensuring efficient and responsive interaction.
- **Connection and Disconnection Handling:** Upon a new client connection, the server dynamically creates a handler for managing the communication with that client. It is equipped to handle client disconnections seamlessly, terminating the dedicated thread once a client disconnects, thereby concluding their session.
- **Proactor Pattern Implementation:** In the advanced stage of the assignment (Section C), the server is re-implemented using a general proactor pattern developed in Section B. This model enhances the server capability to manage connections more effectively, by creating threads on-the-fly based on actual connection requests, thus optimizing resource utilization and scalability.

### Client

- **Dual-threaded Design:** The client features a dual-threaded architecture, with one thread dedicated to listening to server communications and another for keyboard input. This design eliminates the need for busy waiting, ensuring that the user interface remains responsive and efficient.
- **Testing Server Functionality:** The client serves as a critical component for testing the server's functionality, particularly its ability to handle concurrent connections and manage communication threads effectively. It is essential for demonstrating the server's adherence to the designed specifications and its capability to handle real-world chat scenarios.

## How to Compile and Run
### Compiling the Server and Client
For the server, you should do the following:
1. Open a terminal and navigate to the directory containing the `server.c` file.
2. Run the following command to compile the server application:
   ```bash
   gcc -o server server.c -lpthread

#### Compiling the Client
In another terminal you should write the following:
1. ```bash
      gcc -o client client.c -lpthread

#### Connect another clients to server 
Please note that you can connect another clients by just open another terminals and run the following command:
1. ```bash
    gcc -o client client1.c -lpthread

## Part B
In this section, we develop a universal proactor that monitors one or more sockets (representing client connections). 
Upon detecting a new connection, the proactor initiates a fresh thread dedicated to managing the interaction with that specific client. 
The exact count of threads remains variable, adjusting dynamically in response to the flux of incoming connections. 
The focus of this segment is on establishing the overarching proactor mechanism and the construction of the corresponding proactor library.

## Part C
In this segment, we will operate the server initially created in Part A, now integrated with the general proactor framework devised in Part B.

First, in this part you need to open a terminal in the right folder and run the following:
1. ```bash
    make all

#### 
After that, get into Section 3 in each terminal with cd Section3 command on terminal:
1. ```bash
    cd Section3
####
Next, run these commands for the server:
1. ```bash
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
    
2. ```bash
    ./proactor_server

####
Now, to run the clients, you need to run the following command:
1. ```bash
    ./client

####

### Acknowledgments

Thanks to all the contributors who have invested their time in improving this assignment and make it all work well:
Jonatan boritsky, Avi rahimov, Avichay mazin.

