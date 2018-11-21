import serial
import binascii

ser = serial.Serial


# Class connection makes the connection with the arduino and does stuff with it
class Connection:

    def __init__(self, com='COM3', baudrate=19200, timeout=1):
        global ser
        # Try to get a connection with the arduino
        try:
            ser = serial.Serial(com, baudrate=baudrate, timeout=timeout)
            self.readdata(ser)

        except serial.serialutil.SerialException:
            print('cant find connection')

    def readdata(self, ser):

        # num for x in graph
        num = 1
        numlicht = 1

        while ser.read:
            data = ser.readline()
            # Hexlify the data to hex values instead of binary
            datatohex = binascii.hexlify(data)
            # Convert Hex value to ascii
            datatoasc = datatohex[0:6].decode('ascii')

            type_data = datatoasc[0:1]
            waarde = datatoasc[1:3]

            # get rid of empty posts
            if type_data is not '':
                code = (int(type_data, 16))
                value = (int(waarde, 16))
                # when code == 8 it's temp, 4 == light
                if code == 8:
                    listforgraph = [num, value]
                    self.write('temp.txt', listforgraph)
                    num += 1
                elif code == 4:
                    lichtlistforgraph = [numlicht, value]
                    self.write('licht.txt', lichtlistforgraph)
                    # add up num to make data for graph
                    numlicht += 1

    # Write data to files
    def write(self, filename, listtype):
        i_run_once()
        file = open(filename,'a')
        file.write(''.join(str(listtype).strip("[]")) + '\n')
        file.close()


i_run_once_has_been_run = False


def go_up1():
    global ser
    print("up")
    ser.write(b"o")


def go_down():
    global ser
    print("down")
    ser.write(b"d")


# function to clear text document on startup (runs only once, hence the name)
def i_run_once():
    global i_run_once_has_been_run

    if i_run_once_has_been_run:
        return

    open('temp.txt', 'w').close()
    open('licht.txt', 'w').close()

    i_run_once_has_been_run = True
