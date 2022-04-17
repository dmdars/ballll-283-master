import telnetlib
import time
import serial

ipAddress = "192.168.43.212"
port = 28097

ser = serial.Serial("COM5", 9600)

tn = telnetlib.Telnet()
tn.open(ipAddress, port)

time.sleep(3)

count = 0
while True:
    if count == 0:
        tn.read_until(b"1s")
        # print('W') 
        tnFlag = 's'
        print(tnFlag)
        ser.write("s".encode('utf_8'))
    else:
        tn.read_until(b"s")
        # print('W') 
        tnFlag = 's'
        print(tnFlag)
        ser.write("s".encode('utf_8'))

    tn.read_until(b'S')
    # print('S')
    tnFlag = 'S'
    print(tnFlag)
    ser.write("S".encode('utf_8'))

    count = count + 1

tn.close()
ser.close()