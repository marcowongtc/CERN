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


#Initialize the accumulated list for all HV
Leading_Time_Accumlist = np.array([])
Trailing_Time_Accumlist = np.array([])
Time_width_Accumlist = np.array([])
Peak_No_Accumlist = np.array([])

#initialize the accumulated Distinct Multiple Peak and Bad Signal
DistinctMulPeak_TotCount = 0
BadSignal_TotCount = 0

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
    DistinctMulPeak_Count = 0
    BadSignal_Count = 0

    #initialize total list for one signal
    Leading_Time_Totlist = np.array([])
    Trailing_Time_Totlist = np.array([])
    Time_width_Totlist = np.array([])
    Peak_No_Totlist = np.array([])
    


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
                    Leading_Time_list = np.array([])
                    Trailing_Time_list = np.array([])


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
                                        Leading_Time_list = np.append(Leading_Time_list, Leading_Time)
                                        print("\n")
                                        print(filename)
                                        print("zero! leading have!")
                                        break

                                    if CurrentValue > 0 and shifted_channel_column_values[j] < 0:
                                        Trailing_Time = TimeforZero/TimeforZero_count
                                        Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)
                                        print("zero! trailing have!")
                                        break
                                    
                                    if CurrentValue > 0 and shifted_channel_column_values[j] > 0:
                                        Trailing_Time = time_column_values[i]
                                        Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)                                       
                                        Leading_Time = time_column_values[j]
                                        Leading_Time_list = np.append(Leading_Time_list, Leading_Time)
                                        print("zero! trailing have!")
                                        print("\n")
                                        print(filename)
                                        print("zero! leading have!")
                                        break

                                    if CurrentValue < 0 and shifted_channel_column_values[j] < 0:
                                        Leading_Time = time_column_values[i]
                                        Leading_Time_list = np.append(Leading_Time_list, Leading_Time) 
                                        Trailing_Time = time_column_values[j]
                                        Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)                                       
                                        print("\n")
                                        print(filename)
                                        print("zero! leading have!")
                                        print("zero! trailing have!")
                                        break
                                    
                        #for simple cases two point is crossing the zero line (HM)
                        if CurrentValue < 0 and NextValue > 0:
                            Leading_Time = (time_column_values[i]+time_column_values[i+1])/2 
                            Leading_Time_list = np.append(Leading_Time_list, Leading_Time)
                            print("\n")
                            print(filename)
                            print("leading have!")

                        if CurrentValue > 0 and NextValue < 0:
                            Trailing_Time = (time_column_values[i]+time_column_values[i+1])/2 
                            Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)
                            print("trailing have!")
                    
                    print("Leading Time:{}".format(Leading_Time_list))
                    print("Trailing Time:{}".format(Trailing_Time_list))

                
                    if len(Leading_Time_list) == len(Trailing_Time_list):
                        
                        if len(Leading_Time_list) == 1:
                            Leading_Time_Totlist = np.append(Leading_Time_Totlist, Leading_Time_list[0])
                            Trailing_Time_Totlist = np.append(Trailing_Time_Totlist, Trailing_Time_list[0])
                            Time_width_Totlist = np.append(Time_width_Totlist, Trailing_Time_list[0] - Leading_Time_list[0])
                            Peak_No_Totlist = np.append(Peak_No_Totlist, 1)
                        if len(Leading_Time_list) > 1:
                            Leading_Time_Totlist = np.append(Leading_Time_Totlist, Leading_Time_list[0])
                            Trailing_Time_Totlist = np.append(Trailing_Time_Totlist, Trailing_Time_list[-1])
                            Time_width_Totlist = np.append(Time_width_Totlist, Trailing_Time_list[-1] - Leading_Time_list[0])
                            Peak_No_Totlist = np.append(Peak_No_Totlist, len(Leading_Time_list))
                            DistinctMulPeak_Count += 1
                            print("Distinct Multiple Peak!")
                    
                    else:
                        BadSignal_Count +=1
                        print(filename)
                        print("Bad Signal\n\n")
                    
                    #print(Time_width_Totlist)
                    break

            # Progress counter
            Ch1FileCount += 1
        
            if Ch1FileCount % Ninterval == 0:
                print("Ch1 progress:{}/{}".format(Ch1FileCount,Ntot))

    #Accumulated MaxNum_Time for all HV
    Leading_Time_Accumlist = np.append(Leading_Time_Accumlist, Leading_Time_Totlist)
    Trailing_Time_Accumlist = np.append(Trailing_Time_Accumlist, Trailing_Time_Totlist)
    Time_width_Accumlist = np.append(Time_width_Accumlist, Time_width_Totlist)
    Peak_No_Accumlist = np.append(Peak_No_Accumlist, Peak_No_Totlist)

    #Accumulated Number for DistinctMultiplePeak + BadSignal
    BadSignal_TotCount += BadSignal_Count
    DistinctMulPeak_TotCount += DistinctMulPeak_Count
   
                            
    #Summary
    print("\nSummary:")
    print("Ch1 Muon Count: {} ".format(Ch1MuonCount))
    print("Ch1 Analysis Count: {}".format(len(Time_width_Totlist)))
    print("Distinct Multiple Peak Count: {}".format(DistinctMulPeak_Count))
    print("Bad Signal Count: {}".format(BadSignal_Count))


    # save the array to a file - each HV maxNum_time 
    np.savetxt("Ch1LeadingTime_{}HV_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), Leading_Time_Totlist, delimiter=',')
    np.savetxt("Ch1TrailingTime_{}HV_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), Trailing_Time_Totlist, delimiter=',')
    np.savetxt("Ch1TimeWidth_{}HV_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), Time_width_Totlist, delimiter=',')
    np.savetxt("Ch1PeakNo_{}HV_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), Peak_No_Totlist, delimiter=',')


    print("Leading Time list:{}".format(Leading_Time_Totlist))
    print("Trailing Time list:{}".format(Trailing_Time_Totlist))
    print("Time Width list:{}".format(Time_width_Totlist))
    print("Peak No list:{}".format(Peak_No_Totlist))



#Final summary
print("-------------------------------------")
print("\nFinal Summary:")
print("Total Analysis Count: {}".format(len(Time_width_Accumlist)))
print("Total Distinct Multiple Peak: {}".format(DistinctMulPeak_TotCount))
print("Total Bad Signal: {}".format(BadSignal_TotCount))


# save the array to a file - Accumulated maxNum_time
np.savetxt("Ch1AccumLeadingTime_{}Vth_{}Vamp.txt".format(Vth,Vamp), Leading_Time_Accumlist, delimiter=',')
np.savetxt("Ch1AccumTrailingTime_{}Vth_{}Vamp.txt".format(Vth,Vamp), Trailing_Time_Accumlist, delimiter=',')
np.savetxt("Ch1AccumTimeWidth_{}Vth_{}Vamp.txt".format(Vth,Vamp), Time_width_Accumlist, delimiter=',')
np.savetxt("Ch1AccumPeakNo_{}Vth_{}Vamp.txt".format(Vth,Vamp), Peak_No_Accumlist, delimiter=',')


print("Accum Leading Time list:{}".format(Leading_Time_Accumlist))
print("Accum Trailing Time list:{}".format(Trailing_Time_Accumlist))
print("Accum Time Width list:{}".format(Time_width_Accumlist))
print("Accum Peak No list:{}".format(Peak_No_Accumlist))


#---------------------------
##--Convention:-------------
##--List: one signal--------
##--TotList: one HV---------
##--AccumList: all HV-------
#---------------------------