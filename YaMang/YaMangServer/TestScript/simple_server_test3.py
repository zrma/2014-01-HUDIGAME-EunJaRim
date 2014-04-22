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
PKT_CS_CHAT = 3;
PKT_CS_CHAT = 3;
PKT_CS_GAMEOVER = 5;
PKT_CS_ROOM_CREATE = 7;
PKT_CS_ROOM_CHANGE = 9;
PKT_CS_GENERATE_CORPS = 100;
PKT_CS_MOVE_CORPS = 102;
PKT_CS_CORPS_CHANGE_FORMATION = 200;

UNIT_ARROW = 10;
UNIT_GUARD = 20;
UNIT_KNIGHT = 30;
UNIT_PIKE = 40;
UNIT_SWORD = 50;

def str_generator(size=128, chars=string.ascii_uppercase + string.digits):
        return ''.join(' ');

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
print 'Connecting...';
s.connect((HOST, PORT));


s.send(struct.pack('hhi', 8, PKT_CS_LOGIN, pid));

while True:
        try:
                mode = input("[SELECT MODE- EXIT:0, CHAT:3, GAMEOVER:5 \n ROOM_CREATE:7, ROOM_CHANGE:9, GENERATE_CORPS:100 \n Corps_CHANGE_FORMATION:200]");
        except:
                continue;

        if mode == 0:
                break;
        elif mode == PKT_CS_CHAT:
                message = raw_input("message:");
                message = message + str_generator(1000-len(message));
                s.send(struct.pack('<hhi1024s', 1032, mode, pid, message));
        elif mode == PKT_CS_GAMEOVER:
                targetID = input("TargetID:");
                isWON = input("isWON?[Win = 1 Lose = 0]:");
                s.send(struct.pack('<hhi?', 9, mode, targetID, bool(isWON)));
        elif mode == PKT_CS_ROOM_CREATE:
                s.send(struct.pack('hhi', 8, mode, pid));
        elif mode == PKT_CS_ROOM_CHANGE:
                roomFrom = input("ROOM FROM:");
                roomTO = input("ROOM TO:");
                whoMove = input("WHO MOVE:");
                s.send(struct.pack('hhiii', 16, mode, roomFrom, roomTO, whoMove));
        elif mode == PKT_CS_GENERATE_CORPS:
                typeNum = input("UnitType[ARROW:10, GUARD:20, KNIGHT:30, PIKE:40, SWORD:50]:");
                posX = float(raw_input("PosX: "));
                posZ = float(raw_input("PosZ: "));
                s.send(struct.pack('hhiff', 16, mode, typeNum, posX, posZ));
        elif mode == PKT_CS_CORPS_CHANGE_FORMATION:
                corpsID = input("corpsID:");
                typeNum = input("FormationType[DEFENSE:10, DESTROY:20, RUSH:30]:");
                s.send(struct.pack('hhii', 12, mode, corpsID, typeNum));
                
print "close!";
s.close()
