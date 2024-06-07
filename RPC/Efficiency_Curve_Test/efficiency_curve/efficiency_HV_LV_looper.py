import os
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt

#define a fucntion to read csv file and append the value of a column into a list
def read_csv(csv_file):
    
    with open(csv_file, 'r') as csv_file: #with open: will help u close the file as u finish
        csv_reader = csv.reader(csv_file)
        column_values = []

        for row in csv_reader:
            column_values.append(float(row[4]))
        
        return column_values

#define a function to count the Fileno of HV and check whether the Ch1no = Ch2no
def countFile(HV, Vth):
    NtotCh1 = 0
    NtotCh2 = 0
    Ntot = 0
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



#Set Desired Voltage
HV_list = V_GenList(4.8, 6.0, 0.1)
Vth_list = V_GenList(1.8, 2.2, 0.4)
Vamp_list = [1.3]
print("HV list: {}".format(HV_list))
print("Vth list: {}".format(Vth_list))
print("Vamp list: {}".format(Vamp_list))


#Set list for plotting
#np.arange(total entities) -> will reshape(n:Vth -row ,m:HV -col) afterwards
eff_array = np.array([])
err_array = np.array([])


#Set Progress interval
Ninterval = 250


#for loop for all HV data prepared, here we only analyzed one HV
for Vth in Vth_list:
    print("{}Vth_{}Vamp_dataset".format(Vth,Vamp_list[0])) #here assume Vamp_list has only one entry
    for HV in HV_list:


        #word
        
        print("--------------Working----------------")
        print("HV:{}kV".format(HV))

        #Count Eventno and check Ch1No <-> Ch2No
        Ntot = countFile(HV, Vth)
    
    

        #Word
        print("Scanning {} events in the directory for CH1".format(Ntot))

        #Initialize a counter for the number of files that meet the criteria
        Ch1MuonCount = 0
        Ch1FileCount = 0

        # Loop through all files in the directory
        for filename in FilenameList:
            if filename.endswith('.csv') and "Ch1" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename:
                
                # Open the CSV file and read the specified column
                Path = os.path.join(directory_path, filename)
                column_values = read_csv(Path)

                # Check if any value in the column is greater than 0.6
                for value in column_values:
                    if value > 0.6:
                        Ch1MuonCount += 1
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

        #Print Efficiency and error bar
        if Ch2MuonCount != 0:
            efficiency = Ch1MuonCount / float(Ch2MuonCount)
            errorbar = np.sqrt(efficiency*(1-efficiency)/float(Ch2MuonCount))
            print("Efficiency: {} ".format(efficiency))
            print("Error Bar:{}".format(errorbar))

            eff_array = np.append(eff_array, efficiency)
            err_array = np.append(err_array, errorbar)
            #np.append: return a new updated array


#reshape the arange reshape(n:Vth -row ,m:HV -col)
eff_array = eff_array.reshape(len(Vth_list), len(HV_list))
err_array = err_array.reshape(len(Vth_list), len(HV_list))
print("eff_array: {}".format(eff_array))
print("err_array: {}".format(err_array))

# open a binary file in write mode
eff_file = open("eff_{}Vth_{}Vamp".format(Vth_list,Vamp_list), "wb")
err_file = open("err_{}Vth_{}Vamp".format(Vth_list,Vamp_list), "wb")
np.save(eff_file, eff_array)
np.save(err_file, err_array)
eff_file.close
err_file.close


 
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