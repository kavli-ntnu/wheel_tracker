## Read write encoder
import sys,os
from datetime import datetime,timezone,timedelta
import serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import multiprocessing
from scipy.ndimage.filters import gaussian_filter1d
from tkinter import filedialog
import csv


port = 'COM12'
max_num_values = 600
ser = serial.Serial(port, 250000, timeout=.05)

# initialize output csvfile
output_csv = None

# boolean to check if values should be written to .csv
write2csv = False

positions = []
positions_diff = [0]

win = pg.GraphicsWindow()
win.setWindowTitle('Wheel position')
p = win.addPlot()
p_diff = win.addPlot()

# Create pens (display properties lines)
pen_curve = pg.mkPen('g', width=2, style=QtCore.Qt.SolidLine)
pen_curve_diff = pg.mkPen('w', width=2, style=QtCore.Qt.SolidLine)

p.addLegend();p_diff.addLegend()

# Create curves
curve = p.plot(np.zeros(100),pen=pen_curve,name='Abs position')
curve_diff = p_diff.plot(np.zeros(100),pen=pen_curve_diff,name='Rel position')


def update_plot():
    global p,curve
    if len(positions)>1:
        curve.setData(positions[-max_num_values:])
        # add values to the difference plot - filter a snippet with 1D gaussian
        filtered_diff = gaussian_filter1d(positions_diff[-max_num_values:],1.5,mode='nearest')
        curve_diff.setData(filtered_diff)

def update():
    write2csv = True
    line = ser.readline().decode("utf-8")
    if not line:
        write2csv = False

    #print('{}:   {}'.format(datetime.strftime(datetime.now(),'%H:%M:%S'),line))
    if write2csv:
        new_value = int(line)
        positions.append(new_value)
        if output_csv:
            with open(output_csv, 'a') as csvfile:
                filewrite = csv.writer(csvfile, delimiter=',', quoting=csv.QUOTE_MINIMAL)
                filewrite.writerow([datetime.now(),new_value])

        if len(positions) > 1:
            positions_diff.append(positions[-1]-positions[-2])
    # Call the update!
    update_plot()

# Attach timer and call update function every 15 ms
timer = pg.QtCore.QTimer()
timer.timeout.connect(update)
timer.start(15)

if __name__ == '__main__':
    print('Rotary encoder to .csv')
    options = {}
    options['defaultextension'] = '.csv'
    options['initialdir'] = 'C:/DATA_TEMP/wheel'
    options['initialfile'] = datetime.now().strftime("%H-%M-%S_%m-%d-%Y")
    options['title'] = 'Choose export .csv file'
    output_csv = filedialog.asksaveasfilename(**options)
    print(output_csv)
    #print(datetime.strftime(datetime.now(),'%d.%h %Y | %H:%M:%S'))

    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
