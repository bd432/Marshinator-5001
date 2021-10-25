import serial
import time
import matplotlib.pyplot as plt

ser = serial.Serial("COM6", 4800)
print("Start program")

r_list = []
theta_list = []
der_list = []
x_list = []
y_list = []
i = 0

time.sleep(0.5)
ser.write('0'.encode('utf-8'))

while True:
    serial_str=str(ser.readline())[2:-5]

    if serial_str[:4] == "R - ":
        r = float(serial_str[4:])
        if r > 100:
            r_list.append(100)
        else:
            r_list.append(r)
        #print(r)
        i = 0

    if serial_str[:6] == "Der - ":
        der = float(serial_str[6:])
        der_list.append(der)
        i = 0

    if serial_str[:8] == "Theta - ":
        theta = float(serial_str[8:])
        theta_list.append(theta)
        #print(theta)
        i = 0

    if serial_str[:4] == "X - ":
        x = float(serial_str[4:])
        x_list.append(x)
        #print(theta)
        i = 0
    if serial_str[:4] == "Y - ":
        y = float(serial_str[4:])
        y_list.append(y)
        #print(theta)
        i = 0

    if serial_str[:3] == "R- ":
        print(serial_str)
        i = 0
    if serial_str[:7] == "Theta- ":
        print(serial_str)
        i = 0

    if serial_str == "Failed":
        print("Failed")
        i = 0

    if serial_str[:10] == "Peak no - ":
        print(serial_str)
        i = 0
    if serial_str[:11] == "Block no - ":
        print(serial_str)
        i = 0
    if serial_str[:9] == "Width1 - ":
        print(serial_str)
        i = 0

    i =  i + 1
    if i > 200:
        break

ser.close()
#print("Len R : " + str(len(r_list)))
#print("Len theta : " + str(len(theta_list)))
#print("Len der : "+ str(len(der_list)))

dot_list = []

for i in range(len(x_list)-2):
    dot = (x_list[i+1]-x_list[i])*(x_list[i+2]-x_list[i+1]) + (y_list[i+1]-y_list[i])*(y_list[i+2]-y_list[i+1])
    dot_list.append(dot)

avr_list = []

for i in range(len(dot_list)-3):
    avr = (dot_list[i] + dot_list[i+1] + dot_list[i+2] + dot_list[i+3])/4
    avr_list.append(avr)


fig, axs = plt.subplots(2, 2)


axs[0,0].set_title('Radar output')
axs[0,0].plot(theta_list, r_list)

#axs[0,1].set_title('Derivative')
#axs[0,1].plot(theta_list[1:-2], der_list)

axs[1,0].scatter(x_list,y_list)
axs[1,0].set_title('Plot')

axs[1,1].set_title('Dot of adjacent vectors')
axs[1,1].set_ylim(0,10)
axs[1,1].plot(theta_list[1:-1], dot_list)

axs[0,1].set_title('Average Dot of adjacent vectors')
axs[0,1].set_ylim(0,5)
axs[0,1].plot(theta_list[1:-4], avr_list)

manager = plt.get_current_fig_manager()
manager.full_screen_toggle()
plt.show()

print("Terminated")
