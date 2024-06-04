import os
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt

#define a fucntion to read csv file and append the value of a column into a list
#ColNo: 3 - time, 4 - value of channel
def read_csv(csv_file, ColNo):
    
    with open(csv_file, 'r') as csv_file: #with open: will help u close the file as u finish
        csv_reader = csv.reader(csv_file)
        column_values = np.array([])

        for row in csv_reader:
            column_values =np.append(column_values, float(row[ColNo]))
        
        return column_values
    

#define a function to count the Fileno of HV and check whether the Ch1no = Ch2no
def countFile(HV, Vth, Vamp):
    Ntot = 0
    print("---------------------------")

    #Counting number of events
    print("#Counting no. of events")
    for filename in FilenameList:
        if filename.endswith('.csv') and "Ch2" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename and str(str(Vamp) + "Vamp") in filename:
            Ntot += 1
    print("Ch2 EventNo: {}".format(Ntot))  
    return Ntot


#define a funciton to generate voltage list
def V_GenList(V_min, V_max, V_interval):
    V_list = []
    V = V_min
    while V <= V_max:
        V_list.append(V)
        V += V_interval
    return V_list



#Set the path to the current directory
directory_path = os.getcwd()
FilenameList = os.listdir(directory_path)

#Set desired Voltage
HV_list = V_GenList(4.8, 6.0, 0.1)
Vth = 2.2
Vamp = 1.5
print("HV list: {}".format(HV_list))
print("Vth: {}".format(Vth))
print("Vamp: {}".format(Vamp))


#Initialize the accumulated list
MaxNum_Time_Accumlist = np.array([])

#looping all HV - MaxNum_time
for HV in HV_list:

    #word
    print("--------------Working----------------")
    print("HV:{}kV".format(HV))
    
    #count number of file
    Ntot = countFile(HV,Vth,Vamp)
    Ch2FileCount = 0
    Ch2MuonCount = 0
    Ninterval = 100

    #Word
    print("Scanning {} events in the directory for CH2".format(Ntot))

    #Maximum analysis
    MaxNum_Time_list = np.array([])
    for filename in FilenameList:
        if filename.endswith('.csv') and "Ch2" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename and str(str(Vamp) + "Vamp") in filename:
        
            # Open the CSV file and read the specified column
            Path = os.path.join(directory_path, filename)
            time_column_values = read_csv(Path,3)
            channel_column_values = read_csv(Path, 4)

            abs_channel_column_values = np.abs(channel_column_values)

            for value in abs_channel_column_values:
                if value > 0.4:
                    Ch2MuonCount += 1
                
                    #Get the minimum of channel2 value <-> corrsponding time
                    max_num = max(abs_channel_column_values)
                    max_index = abs_channel_column_values.argmax()
                    max_num_time = time_column_values[max_index]

                    MaxNum_Time_list = np.append(MaxNum_Time_list, float(max_num_time))
                    break
            
            #Accumulated MaxNum_Time
            MaxNum_Time_Accumlist = np.concatenate((MaxNum_Time_Accumlist, MaxNum_Time_list))

            # Progress counter
            Ch2FileCount += 1
        
            if Ch2FileCount % Ninterval == 0:
                print("Ch2 progress:{}/{}".format(Ch2FileCount,Ntot))
            

    print("Ch2 Muon Count: {} ".format(Ch2MuonCount))


    # save the array to a file - each HV maxNum_time 
    np.savetxt("Ch2max_{}HV_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), MaxNum_Time_list, delimiter=',')


# save the array to a file - Accumulated maxNum_time
np.savetxt("Ch2maxAccum_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), MaxNum_Time_Accumlist, delimiter=',')





    