#! /usr/bin/python
# ec.py - A simple echo client to talk to our game's server
# Brian Lawson
# CS 3440 - Spring 2015
# 22 Jan 2015

import socket

def main():
	host = 'rookie.cs.dixie.edu'
	port = 3333
	size = 1024

	# create the socket then connect it to the echo server
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect( (host, port) )
	msg = None

	while True:
		# prompt user for message to send
		msg = raw_input("Enter a message to be sent (q to end): ")
		if msg == 'q':
			print 'Bye bye'
			break;

		if msg != '':
			s.send(msg)
			data = s.recv(size)
			print 'Received:', data

	s.close()

if __name__ == '__main__':
	main()
