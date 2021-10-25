#Reads data drom COM^ Serial and adds to graph if given trigger word of "Py " at the start of a println


import serial
import numpy as np
import matplotlib.pyplot as plt
import datetime as dt
import matplotlib.animation as animation

ser = serial.Serial("COM6", 9600)
print("Start program")


# Parameters
x_len = 50         # Number of points to display
y_range = [0, 50]  # Range of possible Y values to display
# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = list(range(0, x_len))
ys = [0] * x_len
ax.set_ylim(y_range)

# Create a blank line. We will update the line in animate
line, = ax.plot(xs, ys)

# Add labels
plt.title('Ultrasound input')
plt.xlabel('Samples')
plt.ylabel('Distance (cm)')

# This function is called periodically from FuncAnimation
def animate(i, ys):

    cc=str(ser.readline())
    if cc[2:5] == "Py ":
        ys.append(float(cc[5:][:-5]))
        #print(cc[5:][:-5])

    ys = ys[-x_len:]
    # Update line with new Y values
    line.set_ydata(ys)

    return line,

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig,animate,fargs=(ys,),interval=50,blit=True)
plt.show()

cc=str(ser.readline())
if cc[2:5] == "Py ":
    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys.append(float(cc[5:][:-5]))
    print(cc[5:][:-5])
