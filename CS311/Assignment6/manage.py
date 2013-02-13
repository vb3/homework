#! /usr/bin/python
import socket
import select
import string
import math

class Manage:
    
    def __init__( self, port ):
        self.port = port;
        self.srvsock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
        self.srvsock.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1 )
        self.srvsock.bind( ("", port) )
        self.srvsock.listen( 5 )
        self.perfect_nums = []
        
        self.descriptors = [self.srvsock]
        print 'PerfectNumberServer started on port %s' % port
    
    def run( self ):
        last = 1
        
        while 1:
            # Await an event on a readable socket descriptor
            (sread, swrite, sexc) = select.select( self.descriptors, [], [] )
            
            # Iterate through the tagged read descriptors
            for sock in sread:
            
                # Received a connect to the server (listening) socket
                if sock == self.srvsock:
                    self.accept_new_connection()
                else:
                
                    # Received something on a client socket
                    data = sock.recv(512)
                    # Check to see if the peer socket closed
                    if data == '':
                        host,port = sock.getpeername()
                        data = 'Client left %s:%s\r\n' % (host, port)
                        print data
                        #self.broadcast_string( data )
                        sock.close
                        self.descriptors.remove(sock)
                    else:
                        host,port = sock.getpeername()
                        newstr = '[%s:%s] %s' % (host, port, data)
                        print newstr
                        
                        #if '$' send the list of perfect numbers
                        if(data[0] == '$'):
                            strsend = str(self.perfect_nums)
                            sock.send(strsend)
                            newstr = 'Sent [%s:%s] %s' % (host, port, strsend)
                            print newstr
                            continue
                        
                        #if requesting range, check for '!'
                        exclaim_loc = string.find(data, '!')
                        if( exclaim_loc >= 0 ):
                            print "Client requested next range."
                            toprange = int(data[exclaim_loc+1:]) * 500
                            
                            strsend = str(last) + ","
                            while (toprange > 0):
                                toprange -= (last-1)
                                if(toprange > 0):
                                    last += 1
                                    
                            strsend += str(last)
                            newstr = 'Sent [%s:%s] %s' % (host, port, strsend)
                            sock.send(strsend)
                            print newstr
                            
                            #get rid of toprange
                            if (exclaim_loc == 0):
                                data = ""
                            else:
                                data = data[:exclaim_loc]
                        if(len(data) > 0): #if recieving perfect numbers
                            self.handle_data(data)
                        

    def broadcast_string( self, strSend):
        for sock in self.descriptors:
            if sock != self.srvsock:
                sock.send(strSend)

    def accept_new_connection( self ):
        newsock, (remhost, remport) = self.srvsock.accept()
        self.descriptors.append( newsock )
        
        newsock.send("You're connected to the Python Perfect Number server\r\n")
        strSend = 'Client joined %s:%s\r\n' % (remhost, remport)
        print strSend
        #self.broadcast_string( strSend )

    def handle_data(self, data):
        #if its just list add to perfect_num list
        self.perfect_nums +=  map(int, data[0:-1].split(','))
        self.perfect_nums.sort()
        print "Perfect Numebrs: ", self.perfect_nums

def main():
    myServer = Manage(4659)
    myServer.run()

if __name__ == '__main__':
	main()