import matplotlib.pyplot as plt
import numpy as np
import binascii
from array import array

file_object  = open("MYLOG.TXT", "rb")

# dataContainer - L...unsigned long
dataContainer = array('L')

for i in range(1,20000,4):
    rawValue = file_object.read(4)
    dataContainer.append((rawValue[0]<<24)|(rawValue[1]<<16)|
                         (rawValue[2]<<8)|(rawValue[3]))

plt.figure(facecolor='white')
plt.plot(dataContainer[1:4860], linewidth=2.0)
plt.grid(b=True, which='major', axis='both', linestyle='-')
plt.xlabel('Sample',fontsize=20)
plt.ylabel('Time [ms]',fontsize=20)

print("Number of Samples: " + str(len(dataContainer)))

dt = np.diff(dataContainer[1:4860])
print("Time Difference: " + str(dt[480:510]))

plt.figure(facecolor='white')
plt.plot(dt, linewidth=2.0)
plt.grid(b=True, which='major', axis='both', linestyle='-')
plt.xlabel('Sample',fontsize=20)
plt.ylabel('Time Difference [ms]',fontsize=20)

plt.show()

file_object.close()