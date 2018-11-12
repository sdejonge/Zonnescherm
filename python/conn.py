import serial
import binascii


class Connection:

    def __init__(self, com='COM3', baudrate=19200, timeout=1):
        # Try to get a connection with the arduino
        try:
            ser = serial.Serial(com, baudrate=baudrate, timeout=timeout)
            self.readdata(ser)
            if ser.read:
                print('serial open')

        except serial.serialutil.SerialException:
            print('cant find connection')

    def readdata(self, ser):

        # num for x in graph
        num = 1
        global listforgraph
        global lichtlistforgraph

        while ser.read:
            n = 2
            data = ser.readline()
            # Hexlify the data to hex values instead of binary
            datatohex = binascii.hexlify(data)
            # Convert Hex value to ascii
            datatoasc = datatohex[0:6].decode('ascii')

            # sort out first hex number
            temp = [datatoasc[i:i + n] for i in range(0, 2, n)]
            licht = [datatoasc[i:i + n] for i in range(2, 4, n)]
            temp = temp[0]
            licht = licht[0]

            # get rid of empty posts
            if temp and licht is not '':
                temperature = (int(temp, 16))
                lichtintensiteit = (int(licht, 16))
                listforgraph = [num, temperature]
                lichtlistforgraph = [num, lichtintensiteit]
                # add up num to make data for graph
                num += 1

                self.write('temp.txt', listforgraph)
                self.write('licht.txt', lichtlistforgraph)

    def write(self,filename,listtype):
        i_run_once()
        file = open(filename,'a')
        file.write(''.join(str(listtype).strip("[]")) + '\n')
        file.close()


i_run_once_has_been_run = False

# function to clear text document
def i_run_once():
    global i_run_once_has_been_run

    if i_run_once_has_been_run:
        return

    open('temp.txt', 'w').close()
    open('licht.txt', 'w').close()

    i_run_once_has_been_run = True
