FILES INCLUDED:

1. client.cpp:
Source file for the client
This file holds the implementation of the classes and functions described under client.h
Also holds the main function for the client, which will run through the program.

2. client.h:
Header file for the client source file
Creates the description for the classes
    1. UnixDomainSocket
    2. DomainSocketClient - The class holding all of the functions for a client to connect to the server and transfer between the two.
DomainSocketClient contains the following function:
    RunServer - Runs through the creation of a socket and connection to the server, sends the server the data, and then receives and parses through the returned data, printing it out to the screen.

3. makefile:
makefile to compile the client and the server and create executables for both, and a clean to delete the created executables

4. server.cpp
Source file for the server
This file holds the implementation of the classes and functions described under server.h
Also holds the main function for the server, which will run through the program.

5. server.h
Header file for the server source file
Creates the description for the classes
    1. UnixDomainSocket
    2. DomainSocketServer - The class holding all of the function for a server to create a socket and talk to a client through said socket, as well as parsing files for strings.
DomainSocketServer contains the following functions:
    RunServer - runs through all of the creation of the socket, receiving of data, calling to parse through files, and sending the data back
    searchThroughFile - Searches through each line of the file to find if the passed in string is found in the line
    searchLine - Searches through the passed in line to see if it contains the string being searched for


Code for the creation of UnixDomainSockets referenced from Jeremy Lewis on github. 