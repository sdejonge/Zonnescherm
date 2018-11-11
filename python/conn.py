import serial
import binascii
import thread

class Connection:

    def __init__(self, com='COM3', baudrate=19200, timeout=1):

        ser = serial.Serial(com, baudrate=baudrate, timeout=timeout)
        self.readdata(ser)

    def readdata(self, ser):
        while 1:
            n = 2
            data = ser.readline()
            datatohex = binascii.hexlify(data)
            datatoasc = datatohex[0:6].decode('ascii')

            dataList = [datatoasc[i:i + n] for i in range(0, len(datatoasc), n)]
            print(dataList)
