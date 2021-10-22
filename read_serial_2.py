import serial
import time
import matplotlib.pyplot as plt


ser = serial.Serial("COM6", 9600)
print("Start program")

r_list = []
theta_list = []
i = 0

time.sleep(0.5)
ser.write('0'.encode('utf-8'))

while True:
    serial_str=str(ser.readline())
    print(serial_str[2:-5])
    if serial_str[2:6] == "R - ":
        r = float(serial_str[6:-5])
        r_list.append(r)
        print(r)
        i = 0

    if serial_str[2:10] == "Theta - ":
        theta = float(serial_str[10:-5])
        theta_list.append(theta)
        print(theta)
        i = 0
    i =  i + 1
    if i > 1000:
        break

ser.close()
print("Len R : " + str(len(r_list)))
print("Len theta : " + str(len(theta_list)))

plt.figure(1)
plt.clf()
plt.xlabel('Radar output')
plt.grid()
plt.plot(theta_list, r_list)
plt.legend()
plt.show()



print("Terminated")
