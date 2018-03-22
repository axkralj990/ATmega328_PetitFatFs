import matplotlib.pyplot as plt
import numpy as np
import binascii
from array import array

file_object  = open("MYLOG.TXT", "rb")

# dataContainer - L...unsigned long
dataContainer = array('L')

for i in range(1,5000,2):
    rawValue = file_object.read(2)
    dataContainer.append((rawValue[0]<<8)|(rawValue[1]))

plt.figure(facecolor='white')
plt.plot(dataContainer, linewidth=2.0, marker="o", linestyle="-")
plt.grid(b=True, which='major', axis='both', linestyle='-')
plt.xlabel('Sample',fontsize=20)
plt.ylabel('Time [ms]',fontsize=20)

print("Number of Samples: " + str(len(dataContainer)))

dt = np.diff(dataContainer)
print("Time Difference: " + str(dt[480:510]))

plt.figure(facecolor='white')
plt.plot(dt, linewidth=2.0)
plt.grid(b=True, which='major', axis='both', linestyle='-')
plt.xlabel('Sample',fontsize=20)
plt.ylabel('Time Difference [ms]',fontsize=20)

# dData = dataContainer[13000]-dataContainer[12999]
# print("difference: " + str(dData))

for i in range(0,len(dt),1):
    # print(dt[i])
    if dt[i] > 600:
        print("Value: " + str(dt[i]))
        print("Index: " + str(i))

file_object.close()

plt.show()