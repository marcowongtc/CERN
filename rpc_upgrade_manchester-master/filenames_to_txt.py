import os

# Get the current directory
cwd = os.getcwd()

# Get a list of all the filenames in the current directory
filenames = os.listdir(cwd)

# Open a file for writing
with open('csvFileList.txt', 'w') as file:
    # Loop through each filename and write it to the file
    for filename in filenames:
        if ".csv" in filename:
            file.write(filename + '\n')
