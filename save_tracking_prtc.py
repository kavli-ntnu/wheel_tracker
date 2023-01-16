## Read write encoder
# This version works with "wheel_tracker\only_rotary_teensy_prtc/only_rotary_teensy_prtc.ino"
# on Teensy 3.5
# In addition to saving position / interrupt data, it also saves timestamps whenever a sync pulse is detected
# It writes two files 
# - Position data csv
# - _sync Sync data csv


import datetime
import serial
import os

port = 'COM5'   # NOTE! This will change between computers, and may change when a computer is restarted
baudrate = 9600

# initialize output csvfiles
root_folder = 'C:/temp/wheel/'  
output_pos_csv  = None
output_sync_csv = None

if __name__ == '__main__':
    print('Rotary encoder to .csv')
    ser = serial.Serial(port, baudrate, timeout=.5)
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    now = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    
    output_pos_csv  = ''.join([root_folder,now,'.csv'])
    output_sync_csv = ''.join([root_folder,now,'_sync.csv'])
    
    print(f'OUTPUT POSITION DATA: {output_pos_csv}')
    print(f'OUTPUT SYNC DATA:     {output_sync_csv}')

    # Open position csv and take care of header
    os.makedirs(root_folder, exist_ok=True)
    wheel_log = open(output_pos_csv,'a')
    wheel_log.write('Rotary encoder POSITION DATA\n')
    wheel_log.write('Neuropixel setup\n')
    wheel_log.write('Samping rate: 50 sps\n')
    wheel_log.write('{}\n'.format(now))
    wheel_log.write('timestamp_sys,timestamp,position,interrupt,position_at_interrupt\n')
    
    # Open sync csv and take care of header
    sync_log = open(output_sync_csv,'a')
    sync_log.write('Rotary encoder SYNC DATA\n')
    sync_log.write('Neuropixel setup\n')
    sync_log.write('Samping rate: 50 sps\n')
    sync_log.write('{}\n'.format(now))
    sync_log.write('timestamp_sys,timestamp\n')

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
                    now = datetime.datetime.now().strftime('%Y-%m-%d_%H:%M:%S.%f')
                    sync_log.write('{},{}\n'.format(now, sync_timestamp))

                else:
                    # Position / Interrupt data was received
                    wheel_info_format = wheel_info.split('_')
                    if len(wheel_info_format) != 4:
                        # This happens if an extraneous byte bleeds into the next serial write event / serial write events overlap
                        # Just skip this iteration 
                        continue
                    else:
                        try:
                            sync_timestamp          = int(wheel_info_format[0])
                            wheel_position          = int(wheel_info_format[1])
                            interrupt               = wheel_info_format[2] # Interrupt 1(yes)/0(no)
                            interrupt_position      = wheel_info_format[3] # Position at interrupt
                        except (TypeError, ValueError):
                            # This happens if a wrong format has been pushed over serial ... 
                            continue
                        
                        now = datetime.datetime.now().strftime('%Y-%m-%d_%H:%M:%S.%f')
                        elapsed_time_str = str(datetime.timedelta(seconds=sync_timestamp/1000.)) # Give it some nice formatting
                        print(f'{now.split(".")[0]} | Elapsed time: {elapsed_time_str:<15} | Pos: {wheel_position:>5} | Interrupt? {interrupt}')
                        wheel_log.write(f'{now}, {sync_timestamp}, {wheel_position}, {interrupt}, {interrupt_position}\n')

        except KeyboardInterrupt:
            print('End of measurement.')
            ser.close()
            print('Serial connection closed.')
            wheel_log.close()
            break

