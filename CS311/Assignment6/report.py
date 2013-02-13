#! /usr/bin/python
import socket
import sys
import string

if (len(sys.argv) == 1):
    print "Useage: report.py <IP_Addr>"
    exit()

mySocket = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
mySocket.connect ( ( sys.argv[1], 4659 ) )
recv_data = mySocket.recv(512)
print recv_data

mySocket.send('$Give me all Perfect Numbers')
recv_data = mySocket.recv(512)

print recv_data
