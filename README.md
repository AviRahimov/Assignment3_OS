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