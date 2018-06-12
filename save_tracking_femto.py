## Read write encoder
import sys,os
from datetime import datetime,timezone,timedelta
import serial
import csv
from tkinter import filedialog

port = 'COM9'
max_num_values = 600
ser = serial.Serial(port, 250000, timeout=.05)

# initialize output csvfile
output_csv = None


if __name__ == '__main__':
    print('Rotary encoder to .csv')
    options = {}
    options['defaultextension'] = '.csv'
    options['initialdir'] = 'C:/CODE/WHEEL'
    options['initialfile'] = datetime.now().strftime("%H-%M-%S_%m-%d-%Y")
    options['title'] = 'Choose export .csv file'
    output_csv = filedialog.asksaveasfilename(**options)
    print(output_csv)
    line = ser.readline().decode("utf-8")
    print(line)
