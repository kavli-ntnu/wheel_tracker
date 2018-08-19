## Read write encoder
# conda execute
# env:
#  - python >=3
#  - pyserial


import sys, os
from shutil import copyfile

import configparser
from datetime import datetime,timezone,timedelta
import serial
import csv

port = 'COM14'
max_num_values = 600
baudrate = 250000

temp_csv = os.getcwd().replace('\\','/') + '/temp/temp.csv'
try:
    os.remove(temp_csv)
except OSError:
    pass

if __name__ == '__main__':
    print('Rotary encoder to .csv')
    ser = serial.Serial(port, baudrate, timeout=.5)
    print(temp_csv)
    wheel_log = open(temp_csv,'a')

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

    # Copy the .csv file to correct filepath
    # Open export_params.cfg and retrieve csv filepath
    config = configparser.ConfigParser()
    config.read('export_params.cfg')
    output_csv = config['SETTINGS']['export_path']
    sys.stdout.write('Copying file to {} '.format(output_csv))
    copyfile(temp_csv, output_csv)
    sys.stdout.write('Success.')
