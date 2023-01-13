## Read write encoder
# This version works with "wheel_tracker\only_rotary_teensy_prtc/only_rotary_teensy_prtc.ino"
# on Teensy 3.5
# In addition to saving position / interrupt data, it also saves timestamps whenever a sync pulse is detected


import sys, os
from shutil import copyfile
from datetime import datetime, timezone, timedelta
import serial
import csv
import configparser

port = 'COM11'
baudrate = 500000

# initialize output csvfiles
output_pos_csv  = None
output_sync_csv = None

if __name__ == '__main__':
    print('Rotary encoder to .csv')
    ser = serial.Serial(port, baudrate, timeout=.5)
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    now = datetime.now().strftime("%H-%M-%S_%m-%d-%Y")
    
    root_folder = 'C:/DATA_TEMP/WHEEL/'
    output_pos_csv  = ''.join([root_folder,now,'.csv'])
    output_sync_csv = ''.join([root_folder,now,'_sync.csv'])
    
    print(f'OUTPUT POSITION DATA: {output_pos_csv}')
    print(f'OUTPUT SYNC DATA:     {output_sync_csv}')

    # Open position csv and take care of header
    wheel_log = open(output_pos_csv,'a')
    wheel_log.write('Rotary encoder and motor control POSITION DATA\n')
    wheel_log.write('Neuropixel setup\n')
    wheel_log.write('Samping rate: 50 sps\n')
    wheel_log.write('{}\n'.format(now))
    wheel_log.write('timestamp_sys, timestamp, position, interrupt, position_at_interrupt, motor_enable\n')
    
    # Open sync csv and take care of header
    sync_log = open(output_sync_csv,'a')
    sync_log.write('Rotary encoder and motor control SYNC DATA\n')
    sync_log.write('Neuropixel setup\n')
    sync_log.write('Samping rate: 50 sps\n')
    sync_log.write('{}\n'.format(now))
    sync_log.write('timestamp_sys, timestamp_sync\n')

    while True:
        try:
            serial_line = ser.readline()
            if serial_line:
                wheel_info = serial_line.decode('utf-8').strip()
                
                # Check whether a sync timestamp or position data was received
                if '_' not in wheel_info:
                    # Only a timestamp (synchronisation from PRTC) was received
                    try:
                        sync_timestamp = int(wheel_info)
                    except (TypeError, ValueError):
                        # Could not convert the received data to an integer timestamp
                        continue
                    now = datetime.now().strftime('%Y-%m-%d_%H:%M:%S.%f')
                    sync_log.write('{},{}\n'.format(now, sync_timestamp))

                else:
                    # Position / Interrupt data was received
                    wheel_info_format = wheel_info.split('_')
                    now = datetime.now().strftime('%Y-%m-%d_%H:%M:%S.%f')
                    print('{} | Timestamp [ms]: {} | Pos: {} | Interrupt: {} | Motor: {}'.format(now.split('.')[0], wheel_info_format[0], wheel_info_format[1], wheel_info_format[3],  wheel_info_format[4]))
                    wheel_log.write('{},{},{},{},{}\n'.format(now,wheel_info_format[0],wheel_info_format[1],wheel_info_format[2],wheel_info_format[3], wheel_info_format[4]))

        except KeyboardInterrupt:
            print('End of measurement.')
            ser.close()
            print('Serial connection closed.')
            wheel_log.close()
            break

