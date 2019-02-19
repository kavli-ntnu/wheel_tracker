## Read write encoder
import sys,os
from shutil import copyfile
from datetime import datetime,timezone,timedelta
import serial
import csv
import configparser

port = 'COM11'
max_num_values = 600
baudrate = 230400

# initialize output csvfile
output_csv = None


if __name__ == '__main__':
    print('Rotary encoder to .csv')
    ser = serial.Serial(port, baudrate, timeout=.5)
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    now = datetime.now().strftime("%H-%M-%S_%m-%d-%Y")
    root_folder = 'C:/DATA_TEMP/WHEEL/'
    output_csv = ''.join([root_folder,now,'.csv'])
    print(output_csv)
    wheel_log = open(output_csv,'a')
    wheel_log.write('Rotary encoder and motor control\n')
    wheel_log.write('Miniscope setup\n')
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

    # Copy the .csv file to correct filepath
    # Open export_params.cfg and retrieve csv filepath
    try:
        config = configparser.ConfigParser()
        config.read('export_params.cfg')
        dest_csv = config['SETTINGS']['export_path']
        sys.stdout.write('Copying file to {} '.format(dest_csv))
        copyfile(output_csv, dest_csv)
        sys.stdout.write('Success.')

        try:
            os.remove(output_csv)
        except OSError:
            pass
    except AttributeError as err:
        print(err)
        print('No valid configuration found! Skipping copy step.')
