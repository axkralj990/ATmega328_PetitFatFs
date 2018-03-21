import matplotlib.pyplot as plt
import numpy as np
import binascii
from array import array
import hashlib

file_object  = open("MYLOG.TXT", "r")

print(file_object)

# byteContainer - B...signed char
byteContainer = array('B')
# dataContainer - L...unsigned long
dataContainer = array('L')

for i in range(1,100,1):
    asciiChar = file_object.read(1).strip().decode('hex')
    singleByte = int.from_bytes(asciiChar.encode(), 'big')
    # if (singleByte == 32):
    #     break
    byteContainer.append(singleByte)

    print("Char: " + str(asciiChar))
    print("Value: " + str(singleByte))

for i in range(0,len(byteContainer)-3,4):
    dataContainer.append((byteContainer[i]<<8)|(byteContainer[i+1])|(byteContainer[i+2])|(byteContainer[i+3]))

print(dataContainer)

plt.figure(facecolor='white')
plt.plot(dataContainer, linewidth=2.0)
plt.grid(b=True, which='major', axis='both', linestyle='-')
plt.xlabel('Sample',fontsize=20)
plt.ylabel('Time [ms]',fontsize=20)
plt.show()

file_object.close()