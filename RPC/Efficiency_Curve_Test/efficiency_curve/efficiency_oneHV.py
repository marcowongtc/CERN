import os
import csv
import sys

#define a fucntion to read csv file and append the value of a column into a list
def read_csv(csv_file):
    
    with open(csv_file, 'r') as csv_file: #with open: will help u close the file as u finish
        csv_reader = csv.reader(csv_file)
        column_values = []

        for row in csv_reader:
            column_values.append(float(row[4]))
        
        return column_values



#Set the path to the current directory
directory_path = './'
FilenameList = os.listdir(directory_path)



#Set Desired Voltage
Vth = 2.2
print("-------------------------------------")
print("dataset Vth = {}".format(Vth))
print("Please input HV (KV):")
HV = input()



##Check Eventno in the dataset
NtotCh1 = 0
NtotCh2 = 0
Ninterval = 100

print("\nFor progress bar")
print("---------------------------")

#Counting number of events
print("#Counting no. of events")
for filename in FilenameList:
    if filename.endswith('.csv') and "Ch1" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename:
        NtotCh1 += 1
    if filename.endswith('.csv') and "Ch2" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename:
        NtotCh2 += 1
print("Ch1 EventNo: {}".format(NtotCh1))       
print("Ch2 EventNo: {}".format(NtotCh2)) 

#Terminate the program if the EventNo not match
if NtotCh1 == NtotCh2:
    Ntot = NtotCh1
else:
    print("---------------------------")
    print("-------------------------------------")
    sys.exit("EventNo of Ch1 and Ch2 does not match. Please check the files in directory")


print("---------------------------\n")





#Initialize a counter for the number of files that meet the criteria
Ch1MuonCount = 0
Ch1FileCount = 0

#Word
print("--------------Working----------------")
print("HV:{}kV".format(HV))
print("Scanning {} events in the directory for CH1".format(Ntot))



# Loop through all files in the directory
for filename in FilenameList:
    if filename.endswith('.csv') and "Ch1" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename:
    
        # Open the CSV file and read the specified column
        Path = os.path.join(directory_path, filename)
        column_values = read_csv(Path)

        # Check if any value in the column is greater than 0.6
        i = -1
        for value in column_values:
            i += 1
            if value > 0.6:
                Ch1MuonCount += 1
                print(filename)
                print("index:{}".format(i))
                print("value:{}".format(value))
                break
        
        # Progress counter
        Ch1FileCount += 1
        
        if Ch1FileCount % Ninterval == 0:
            print("Ch1 progress:{}/{}".format(Ch1FileCount,Ntot))
        

                
                

#Word                
print("\nScanning {} events in the directory for CH2".format(Ntot))

# Initialize a counter for the number of files that meet the criteria
Ch2MuonCount = 0
Ch2FileCount = 0


# Loop through all files in the directory
for filename in FilenameList:
    if filename.endswith('.csv') and "Ch2" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename:

        # Open the CSV file and read the specified column
        Path = os.path.join(directory_path, filename)
        column_values = read_csv(Path)    


        # Check if any value in the column is smaller than -0.4
        for value in column_values:
            if value < -0.4:
                Ch2MuonCount += 1
                break


        # Progress counter
        Ch2FileCount += 1

        if Ch2FileCount % Ninterval == 0:
            print("Ch2 progress:{}/{}".format(Ch2FileCount,Ntot))
        
        
    
#Final Result of HV 
#Print the number of files that meet the criteria
print("\nSummary:")
print("Ch1 Muon Count: {} ".format(Ch1MuonCount))
print("Ch2 Muon Count: {} ".format(Ch2MuonCount))

#Print Efficiency
if Ch2MuonCount != 0:
    efficiency = Ch1MuonCount / float(Ch2MuonCount)
    print("Efficiency: {} ".format(efficiency))
    print("-------------------------------------")
 
 #------------------------------------------------------------------------------------------------------------------------------------------------------
##------Bug Solved!!!----------------------------------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------------------------------------------------------
##------bug here solved long time: "Ch1" and str(str(HV)+"KHV") in filename, after "Ch1" alson need 'in filename'(Same Problem as Ch2)-----------------
##------previous actually count all Ch1 and Ch2 to compare the threshold, occasionally there is some Ch1 also has a value <-0.4, thus count once more--
##------Resulting: Ch2:1003----------------------------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------------------------------------------------------


#---------------------------------------------------------------------------------------
##------Running Time Too long Problem Solved!-------------------------------------------
#---------------------------------------------------------------------------------------
#-------it is the matter of the cernbox/local directory, local access faster (6s vs 55s)
#----------------------------------------------------------------------------------------