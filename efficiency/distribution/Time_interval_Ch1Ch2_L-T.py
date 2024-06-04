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
        if filename.endswith('.csv') and "Ch1" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename and str(str(Vamp) + "Vamp") in filename:
            Ntot += 1
    print("Ch1 EventNo: {}".format(Ntot))  
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
directory_path = '/afs/cern.ch/user/t/tswong/efficiency_plot/dataset_1_8_23_efficiency/'
FilenameList = os.listdir(directory_path)

#Set desired Voltage
HV_list = V_GenList(4.8,6.0,0.1)
Vth = 2.2
Vamp = 1.5
print("HV list: {}".format(HV_list))
print("Vth: {}".format(Vth))
print("Vamp: {}".format(Vamp))

#Set Accumulated list for time interval between two signal
Interval_Ch1Ch2_Accumlist = np.array([])


#looping all HV - MaxNum_time
for HV in HV_list:

    #word
    print("--------------Working----------------")
    print("HV:{}kV".format(HV))
    
    #count number of file
    Ntot = countFile(HV,Vth,Vamp)
    Ch1FileCount = 0
    Ch1MuonCount = 0
    Ninterval = 100

    #Leading Time, Trailing Time, Time width analysis
    Filename_Ch1havepeak = np.array([])

    #initialize total list for one signal
    Ch1Trailing_Time_Totlist = np.array([])
    Ch2Leading_Time_Totlist = np.array([])
    Interval_Ch1Ch2_list = np.array([])



    #Word
    print("Scanning {} events in the directory for CH1".format(Ntot))

    #Time analysis
    for filename in FilenameList:
        if filename.endswith('.csv') and "Ch1" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename and str(str(Vamp) + "Vamp") in filename:
            
            # Open the CSV file and read the specified column
            Path = os.path.join(directory_path, filename)
            time_column_values = read_csv(Path,3)
            channel_column_values = read_csv(Path, 4)



            for value in channel_column_values:
                #filter the normal signal that we count as Muon previously
                if value > 0.6:
                    Ch1MuonCount += 1
                
                    #initialize leading time count and trailing time count list for one signal - array is for multiple leading and trailing
                    Ch1Leading_Time_list = np.array([])
                    Ch1Trailing_Time_list = np.array([])



                    #Get the leading and trailing time by half maximum
                    half_max_num = max(channel_column_values) / 2
                    shifted_channel_column_values = channel_column_values - half_max_num
                    #print(shifted_channel_column_values)
                    
                    for i in range(0,len(shifted_channel_column_values)-1,1): #neglect the last two, since it will out of bound for Next, Nextnextvalue
                        CurrentValue = shifted_channel_column_values[i]
                        NextValue = shifted_channel_column_values[i+1]
                        
                        # for cases some points is exactly equal to zero (Halfmax) 
                        if CurrentValue != 0 and NextValue == 0:
                            TimeforZero = 0
                            TimeforZero_count = 0
                            for j in range(i+1,len(shifted_channel_column_values),1):
                                if shifted_channel_column_values[j] == 0:
                                    TimeforZero += time_column_values[j]
                                    TimeforZero_count += 1

                                if shifted_channel_column_values[j] != 0:
                                    if CurrentValue < 0 and shifted_channel_column_values[j] > 0:
                                        Leading_Time = TimeforZero/TimeforZero_count
                                        Ch1Leading_Time_list = np.append(Ch1Leading_Time_list, Leading_Time)
                                        print("\n")
                                        print(filename)
                                        print("zero! leading have!")
                                        break

                                    if CurrentValue > 0 and shifted_channel_column_values[j] < 0:
                                        Trailing_Time = TimeforZero/TimeforZero_count
                                        Ch1Trailing_Time_list = np.append(Ch1Trailing_Time_list, Trailing_Time)
                                        print("zero! trailing have!")
                                        break
                                    
                                    if CurrentValue > 0 and shifted_channel_column_values[j] > 0:
                                        Trailing_Time = time_column_values[i]
                                        Ch1Trailing_Time_list = np.append(Ch1Trailing_Time_list, Trailing_Time)                                       
                                        Leading_Time = time_column_values[j]
                                        Ch1Leading_Time_list = np.append(Ch1Leading_Time_list, Leading_Time)
                                        print("zero! trailing have!")
                                        print("\n")
                                        print(filename)
                                        print("zero! leading have!")
                                        break

                                    if CurrentValue < 0 and shifted_channel_column_values[j] < 0:
                                        Leading_Time = time_column_values[i]
                                        Ch1Leading_Time_list = np.append(Ch1Leading_Time_list, Leading_Time) 
                                        Trailing_Time = time_column_values[j]
                                        Ch1Trailing_Time_list = np.append(Ch1Trailing_Time_list, Trailing_Time)                                       
                                        print("\n")
                                        print(filename)
                                        print("zero! leading have!")
                                        print("zero! trailing have!")
                                        break
                                    
                        #for simple cases two point is crossing the zero line (HM)
                        if CurrentValue < 0 and NextValue > 0:
                            Leading_Time = (time_column_values[i]+time_column_values[i+1])/2 
                            Ch1Leading_Time_list = np.append(Ch1Leading_Time_list, Leading_Time)
                            print("\n")
                            #print(filename[:-5]+"2.csv")
                            print(filename)
                            print("leading have!")

                        if CurrentValue > 0 and NextValue < 0:
                            Trailing_Time = (time_column_values[i]+time_column_values[i+1])/2 
                            Ch1Trailing_Time_list = np.append(Ch1Trailing_Time_list, Trailing_Time)
                            print("trailing have!")
                    
                    print("Leading Time:{}".format(Ch1Leading_Time_list))
                    print("Trailing Time:{}".format(Ch1Trailing_Time_list))

                
                    if len(Ch1Leading_Time_list) == len(Ch1Trailing_Time_list):
                        
                        if len(Ch1Leading_Time_list) == 1:
                            Ch1Trailing_Time_Totlist = np.append(Ch1Trailing_Time_Totlist, Ch1Trailing_Time_list[0])
                            Filename_Ch1havepeak = np.append(Filename_Ch1havepeak, filename)

                        if len(Ch1Leading_Time_list) > 1:
                            Ch1Trailing_Time_Totlist = np.append(Ch1Trailing_Time_Totlist, Ch1Trailing_Time_list[-1])
                            Filename_Ch1havepeak = np.append(Filename_Ch1havepeak, filename)
                    
                    else:
                        print(filename)
                        print("Bad Signal\n\n")
                    

                    break

            # Progress counter
            Ch1FileCount += 1
        
            if Ch1FileCount % Ninterval == 0:
                print("Ch1 progress:{}/{}".format(Ch1FileCount,Ntot))
    
    print(Filename_Ch1havepeak)
    
    for Ch1filename in Filename_Ch1havepeak:
        # Open the CSV file and read the specified column
        Path = os.path.join(directory_path, Ch1filename[:-5]+"2.csv")
        print(Path)
        time_column_values = read_csv(Path,3)
        channel_column_values = read_csv(Path, 4)        

        #Flip the graph along time axis for trigger (Since the signal is flipped originally)
        channel_column_values *= -1
            


        for value in channel_column_values:
            #filter the normal signal that we count as Muon previously

            if value > 0.6:
                print(Ch1filename)
                #initialize leading time count and trailing time count list for one signal - array is for multiple leading and trailing
                Ch2Leading_Time_list = np.array([])
                Ch2Trailing_Time_list = np.array([])



                #Get the leading and trailing time by half maximum
                half_max_num = max(channel_column_values) / 2
                shifted_channel_column_values = channel_column_values - half_max_num
                #print(shifted_channel_column_values)
                    
                for i in range(0,len(shifted_channel_column_values)-1,1): #neglect the last two, since it will out of bound for Next, Nextnextvalue
                    CurrentValue = shifted_channel_column_values[i]
                    NextValue = shifted_channel_column_values[i+1]
                          
                    # for cases some points is exactly equal to zero (Halfmax) 
                    if CurrentValue != 0 and NextValue == 0:
                        TimeforZero = 0
                        TimeforZero_count = 0
                        for j in range(i+1,len(shifted_channel_column_values),1):
                            if shifted_channel_column_values[j] == 0:
                                TimeforZero += time_column_values[j]
                                TimeforZero_count += 1

                            if shifted_channel_column_values[j] != 0:
                                if CurrentValue < 0 and shifted_channel_column_values[j] > 0:
                                    Leading_Time = TimeforZero/TimeforZero_count
                                    Ch2Leading_Time_list = np.append(Ch2Leading_Time_list, Leading_Time)
                                    print("\n")
                                    print(Ch1filename[:-5]+"2.csv")
                                    print("zero! leading have!")
                                    break

                                if CurrentValue > 0 and shifted_channel_column_values[j] < 0:
                                    Trailing_Time = TimeforZero/TimeforZero_count
                                    Ch2Trailing_Time_list = np.append(Ch2Trailing_Time_list, Trailing_Time)
                                    print("zero! trailing have!")
                                    break
                                    
                                if CurrentValue > 0 and shifted_channel_column_values[j] > 0:
                                    Trailing_Time = time_column_values[i]
                                    Ch2Trailing_Time_list = np.append(Ch2Trailing_Time_list, Trailing_Time)                                       
                                    Leading_Time = time_column_values[j]
                                    Ch2Leading_Time_list = np.append(Ch2Leading_Time_list, Leading_Time)
                                    print("zero! trailing have!")
                                    print("\n")
                                    print(Ch1filename[:-5]+"2.csv")
                                    print("zero! leading have!")
                                    break

                                if CurrentValue < 0 and shifted_channel_column_values[j] < 0:
                                    Leading_Time = time_column_values[i]
                                    Ch2Leading_Time_list = np.append(Ch2Leading_Time_list, Leading_Time) 
                                    Trailing_Time = time_column_values[j]
                                    Ch2Trailing_Time_list = np.append(Ch2Trailing_Time_list, Trailing_Time)                                       
                                    print("\n")
                                    print(Ch1filename[:-5]+"2.csv")
                                    print("zero! leading have!")
                                    print("zero! trailing have!")
                                    break
                                    
                    if CurrentValue < 0 and NextValue > 0:
                        Leading_Time = (time_column_values[i]+time_column_values[i+1])/2 
                        Ch2Leading_Time_list = np.append(Ch2Leading_Time_list, Leading_Time)
                        print("\n")
                        print(Ch1filename[:-5]+"2.csv")
                        print("leading have!")

                    if CurrentValue > 0 and NextValue < 0:
                        Trailing_Time = (time_column_values[i]+time_column_values[i+1])/2 
                        Ch2Trailing_Time_list = np.append(Ch2Trailing_Time_list, Trailing_Time)
                        print("trailing have!")
                    
                print("Leading Time:{}".format(Ch2Leading_Time_list))
                print("Trailing Time:{}".format(Ch2Trailing_Time_list))

                
                if len(Ch2Leading_Time_list) == len(Ch2Trailing_Time_list):
                        
                    if len(Ch2Leading_Time_list) == 1:
                        Ch2Leading_Time_Totlist = np.append(Ch2Leading_Time_Totlist, Ch2Leading_Time_list[0])

                    if len(Ch2Leading_Time_list) > 1:
                        Ch2Leading_Time_Totlist = np.append(Ch2Leading_Time_Totlist, Ch2Leading_Time_list[0])
                    
                else:
                    print(Ch1filename[:-5]+"2.csv")
                    print("Bad Signal\n\n")
                    
                break

    
    Interval_Ch1Ch2_list = Ch2Leading_Time_Totlist - Ch1Trailing_Time_Totlist



    #Accumulated MaxNum_Time for all HV
    Interval_Ch1Ch2_Accumlist = np.append(Interval_Ch1Ch2_Accumlist, Interval_Ch1Ch2_list)


   
                            
    #Summary
    print("\nSummary:")
    print("Ch1 Muon Count: {} ".format(Ch1MuonCount))
    print("Ch1 Analysis Count: {}".format(len(Ch1Trailing_Time_Totlist)))
    print("Ch2 Analysis Count: {}".format(len(Ch2Leading_Time_Totlist)))


    # save the array to a file - each HV maxNum_time 
    np.savetxt("Interval_Ch1Ch2_{}HV_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), Interval_Ch1Ch2_list, delimiter=',')



#Final summary
print("-------------------------------------")
print("\nFinal Summary:")
print("Total Analysis Count: {}".format(len(Interval_Ch1Ch2_Accumlist)))


# save the array to a file - Accumulated Interval_Ch1Ch2
np.savetxt("Interval_Ch1Ch2_Accumlist_{}Vth_{}Vamp.txt".format(Vth,Vamp), Interval_Ch1Ch2_Accumlist, delimiter=',')





#---------------------------
##--Convention:-------------
##--List: one signal--------
##--TotList: one HV---------
##--AccumList: all HV-------
#---------------------------