#!/usr/bin/python

# Echo client program
import socket, time
import struct
import string, random


HOST = '127.0.0.1';    # The remote host
PORT = 9001;              # The same port as used by the server

pid = random.randrange(1000, 1080);
print "PID:", pid;
PKT_CS_LOGIN = 1;
PKT_SC_LOGIN = 2;
PKT_CS_CHAT	 = 3;
PKT_SC_CHAT	 = 4;
PKT_CS_GAMEOVER	 = 5;
PKT_SC_GAMEOVER	 = 6;
PKT_CS_ROOM_CREATE	 = 7;
PKT_SC_ROOM_CREATE	 = 8;
PKT_CS_ROOM_CHANGE	 = 9;
PKT_SC_ROOM_CHANGE	 = 10;


def str_generator(size=128, chars=string.ascii_uppercase + string.digits):
	return ''.join(' ');

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
print 'Connecting...';
s.connect((HOST, PORT));


s.send(struct.pack('hhi', 8, PKT_CS_LOGIN, pid));

while True:
	try:
		mode = input("[SELECT MODE- EXIT:0, CHAT:3, GAMEOVER:5, ROOM_CREATE:7]");
	except:
		continue;

	if mode == 0:
		break;
	elif mode == 3:
		message = raw_input("message:");
		message = message + str_generator(1000-len(message));
		s.send(struct.pack('<hhi1024s', 1032, mode, pid, message));
	elif mode == 5:
		message = raw_input("message:");
		message = message + str_generator(1000-len(message));
		s.send(struct.pack('<hhi1024s', 1032, mode, pid, message));
	elif mode == 7:
		s.send(struct.pack('hhi', 8, mode, pid));
	elif mode == 9:
		roomFrom = input("ROOM FROM:");
		roomTO = input("ROOM TO:");
		whoMove = input("WHO MOVE:");
		s.send(struct.pack('hhiii', 16, mode, roomFrom, roomTO, whoMove));
print "close!";
s.close()
