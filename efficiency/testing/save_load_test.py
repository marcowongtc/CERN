import numpy as np

#initialize an array
array = np.array ([[1,2,3],[3,4,5]])

# open a binary file in write mode
file = open("array", "wb")
np.save(file, array)
file.close

# open the file in read binary mode
file = open("array", "rb")
new_array = np.load(file)

print(new_array)

