## Read write encoder
# This version is identical to the original "save_tracking.py" code, 
# with the exception that the "export_params.cfg" file is not read and 
# the file is not moved

import sys, os
from shutil import copyfile
from datetime import datetime, timezone, timedelta
import serial
import csv
import configparser

port = 'COM5'   # NOTE! This will change between computers, and may change when a computer is restarted
baudrate = 9600

# initialize output csvfile
root_folder = 'C:/temp/wheel/' 
output_csv = None


if __name__ == '__main__':
    print('Rotary encoder to .csv')
    ser = serial.Serial(port, baudrate, timeout=.5)
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    now = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    output_csv = ''.join([root_folder,now,'.csv'])
    print(output_csv)
    os.makedirs(root_folder, exist_ok=True)
    wheel_log = open(output_csv,'a')
    wheel_log.write('Rotary encoder and motor control\n')
    wheel_log.write('Neuropixel setup\n')
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
            ser.close()
            print('Serial connection closed.')
            wheel_log.close()
            break


