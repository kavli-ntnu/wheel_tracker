## Read write encoder
import sys,os
from datetime import datetime,timezone,timedelta
import serial
import csv
from tkinter import filedialog

port = 'COM6'
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
    wheel_log.write('Rotary encoder and motor control\n')
    wheel_log.write('Femtonics setup\n')
    wheel_log.write('Samping rate: 50 sps\n')
    wheel_log.write('{}\n'.format(now))
    wheel_log.write('timestamp,position,interrupt,position_at_interrupt,motor_enable\n')

    while True:
        try:
            serial_line = ser.readline()
            if serial_line:
                wheel_info = serial_line.decode('utf-8').strip()
                wheel_info_format = wheel_info.split('_')
                now = datetime.now().strftime('%Y-%m-%d_%H:%M:%S.%f')
                print('{} | Pos: {:5d} | Interrupt: {} | Motor: {}'.format(now.split('.')[0], int(wheel_info_format[0]), (wheel_info_format[1]), wheel_info_format[3]))

                wheel_log.write('{},{},{},{},{}\n'.format(now,wheel_info_format[0],wheel_info_format[1],wheel_info_format[2],wheel_info_format[3]))

        except KeyboardInterrupt:
            print('End of measurement.')
            wheel_log.close()
            break
