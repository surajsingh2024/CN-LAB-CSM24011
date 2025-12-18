# CN-LAB-CSM24011
This repository contains the implementation of Computer Networks Lab Assignments (1–14) as provided in the official CN Lab Manual. All programs are implemented using C, Python, Mininet, TCP/UDP sockets, RAW sockets, and Wireshark.

🔹 CN Lab – Assignment 1
Objective:

To gain first hands on experience of basic Socket Programming.

Exercise:

Write a program to run TCP client and server socket programs where client first
says “Hi” and in response server says “Hello”.

Steps / Hints:

Create two mininet hosts

Open the hosts individually in xterm windows

In one host, run tcpserver program and then in another run the tcpclient

Learning Outcomes:

Basics of TCP client and server programming.

Output:

🔹 CN Lab – Assignment 2
Objective:

To gain experience of TCP Socket Programming for simple applications.

Exercise:

Write a program using TCP socket to implement the following:

i. Server maintains records of fruits in the format:
fruit-name, quantity Last-sold (server timestamp)

ii. Multiple client purchase the fruits one at a time

iii. The fruit quantity is updated each time any fruit is sold

iv. Send regret message to a client if the requested quantity of the fruit is not available

v. Display the customer ids <IP, port> who has done transactions already.
This list should be updated in the server every time a transaction occurs

vi. The total number of unique customers who did some transaction will be
displayed to the customer every time

Steps / Hints:

Use at least two mininet hosts as clients

Server must be kept running using a loop condition

Take another socket (from accept() call) for keeping client information

Server must send the current stock information to the transacting host as queried

Learning Outcomes:

Multiple client’s communication via server socket can be learned.

Output:

🔹 CN Lab – Assignment 3
Objective:

To gain experience of UDP Socket Programming for simple applications.

Exercise:

Redo assignment 2 using UDP socket.

Steps / Hints:

Same as given in Assignment 1.

Learning Outcomes:

Basics of UDP socket programming.

Output:

🔹 Assignment 4
Objective:

To gain knowledge of packet capturing tools and understand header structures.

Exercise:

Install wireshark in a VM (Virtual Machine) environment.
Draw a time diagram to show the steps in the protocols recorded in the captured file
(saved in the .pcap file of wireshark) during a PING operation.
List the L2, L3, L4 header fields that can be extracted from the .pcap file.

Steps / Hints:

Install wireshark in your linux vm with mininet

Start capturing at any interface

Ping any two hosts

Analyze

Learning Outcomes:

Knowledge of packet capturing tools, protocols and headers.

Output:

🔹 Assignment 5
Objective:

To gain knowledge of more packet capturing tools.

Exercise:

Learn and use maximum number of packet generation tools.

Steps / Hints:

Install Iperf, D-ITG etc. and send traffic among mininet hosts.

Learning Outcomes:

Knowledge of packet generation and capturing tools.

Output:

🔹 Assignment 6
Objective:

To gain knowledge of more TCP/IP C libraries.

Exercise:

Develop a simple C based network simulator to analyze TCP traffic.

Steps / Hints:

Use TCP/IP based C libraries including socket to listen to the incoming packets at the Ethernet port

Extract header and data of the incoming packets

Learning Outcomes:

Knowledge of TCP/IP libraries.

Output:

🔹 Assignment 7
Objective:

Client server communication with UDP packets.

Exercise:

Write UDP client server socket program where client sends one/two number(s)
(integer or floating point) to server and a scientific calculator operation
(like sin, cos, *, /, inv etc.) and server responds with the result after evaluating the value of operation as sent by the client.

Server will maintain a scientific calculator.
Detect in the mininet hosts with wireshark if there is any packet loss?
Show it to the TA.

Steps / Hints:

Use Math.h library in the server side to compute mathematical expressions

Client will send expressions like – sin(x), x+y etc.

Use UDP in a graceful manner to detect any packet loss, if occurs

Run wireshark at the mininet host’s ethernet to capture packet and detect losses

Learning Outcomes:

Understanding reliability of UDP for transactions.

Output:

🔹 Assignment 8
Objective:

Using Thread library and TCP sockets.

Exercise:

Write a program in C using thread library and TCP sockets to build a chat server
which enable clients communicating to each other through the chat server.

Message logs must be maintained in the server in a text file.
Each client will see the conversations in real time.
Clients must handled by a server thread. (Keep it like a group chatbox)

Learning Outcomes:

Understanding multi-threading for TCP sockets.

Output:

🔹 Assignment 9
Objective:

File upload and download using TCP.

Exercise:

Write a client server socket program in TCP for uploading and downloading files
between two different hosts.
Also calculate the transfer time in both the cases.

Learning Outcomes:

Use of TCP for file transfer.

Output:

🔹 Assignment 10
Objective:

Using RAW sockets to generate packets.

Exercise:

Write two C programs using raw socket to send
i. TCP packet where TCP payload will contain your roll number
ii. ICMP time stamp messages towards a target IP.

Learning Outcomes:

Use of RAW socket to create custom TCP and ICMP messages.

Use of RAW socket for packet generation.

Output:

🔹 Assignment 11
Objective:

Using RAW sockets to generate TCP flooding attack.

Exercise:

Write a RAW socket program to generate TCP SYN flood based DDoS attack towards an IP address.
Take four mininet hosts as agent devices.

Learning Outcomes:

Use of RAW sockets to generate SYN based flooding attack.

Understanding the pattern of TCP control messages for DDoS attack.

Output:

🔹 Assignment 12
Objective:

Using RAW sockets to generate ICMP flooding attack.

Exercise:

Do the same attack as given in assignment no. 11 with ICMP packets using RAW socket.

Learning Outcomes:

Use of RAW sockets to generate ICMP based flooding attack.

Understanding the pattern of ICMP communication for DDoS attack.

Output:

🔹 Assignment 13
Objective:

To learn packet capturing and analysis.

Exercise:

Create a binary tree topology with 7 switches in mininet.
Capture packets at the root switch.
Write a C program to extract the headers and draw a time diagram to show the protocols displayed in the captured file
(save the .pcap/.pcapng file of wireshark/tshark) during a PING operation.
List the L2, L3, L4 protocols that can be extracted from the .pcap/.pcapng file.

Learning Outcomes:

Learning to analyze packets and its corresponding protocols.

Learning the roles of L2/L3/L4 protocols for a communication.

Output:

🔹 Assignment 14
Objective:

Creating customized topologies in mininet.

Exercise:

Create a custom leaf-spine topology in mininet using python which can be scaled with increasing switch radix.

Learning Outcomes:

Understanding mininet API.

Learning to perform experiments in virtual networks.

Output:
