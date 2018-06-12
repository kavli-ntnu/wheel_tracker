## Read write encoder
import sys,os
from datetime import datetime,timezone,timedelta
import serial
import csv
from tkinter import filedialog

port = 'COM9'
max_num_values = 600
baudrate = 250000

# initialize output csvfile
output_csv = None


if __name__ == '__main__':
    print('Rotary encoder to .csv')
    ser = serial.Serial(port, baudrate, timeout=.5)

    now = datetime.now().strftime("%H-%M-%S_%m-%d-%Y")
    root_folder = 'C:/DATA/WHEEL/'
    output_csv = ''.join([root_folder,now,'.csv'])
    print(output_csv)
    wheel_log = open(output_csv,'a')

    while True:
        try:
            serial_line = ser.readline()
            if serial_line:
                wheel_pos = serial_line.decode('utf-8').strip()
                now = datetime.now().strftime('%Y-%m-%d_%H:%M:%S.%f')
                print('{} | {}'.format(now.split('.')[0],wheel_pos))
                entry_log = ','.join([now, wheel_pos + '\n'])
                wheel_log.write(entry_log)

        except KeyboardInterrupt:
            print('End of measurement.')
            wheel_log.close()
            break
