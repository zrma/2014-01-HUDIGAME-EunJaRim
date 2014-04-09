# Echo client program
import socket, time
import struct
import string, random


HOST = '127.0.0.1'    # The remote host
PORT = 9001              # The same port as used by the server

pid = random.randrange(1000, 1080);
print "PID:", pid
PKT_CS_LOGIN = 1
PKT_SC_LOGIN = 2
PKT_CS_CHAT	 = 3
PKT_SC_CHAT	 = 4
PKT_CS_TEST	 = 5
PKT_SC_TEST	 = 6

def str_generator(size=128, chars=string.ascii_uppercase + string.digits):
    return ''.join(' ')

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
print 'Connecting...'
s.connect((HOST, PORT));


s.send(struct.pack('hhi', 8, PKT_CS_LOGIN, pid));
"""
data = s.recv(1024);
if not data:
    #print 'NOT DATA'
    self.close();
else:
    print data;
"""

while True:
    message = raw_input("message (If you want exit write exit):")

    if message == "exit":
        break;
    else:
        message = message + str_generator(1000-len(message));
        s.send(struct.pack('<hhi1024s', 1032, PKT_CS_CHAT, pid, message));

"""
if not data:
    #print 'NOT DATA'
    self.close();
else:
    print data;
"""

print "close!";
s.close()
