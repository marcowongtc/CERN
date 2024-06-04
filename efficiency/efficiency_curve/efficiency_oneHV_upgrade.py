import os
import csv
import sys
import numpy as np

#Global variable - Set the path to the current directory
directory_path = '/afs/cern.ch/user/t/tswong/efficiency_plot/dataset_1_8_23_efficiency/'
FilenameList = os.listdir(directory_path)


#define a fucntion to read csv file and append the value of a column into a list
#ColNo: 3 - time, 4 - value of channel
def read_csv(csv_file, ColNo):
    
    with open(csv_file, 'r') as csv_file: #with open: will help u close the file as u finish
        csv_reader = csv.reader(csv_file)
        column_values = np.array([])

        for row in csv_reader:
            column_values =np.append(column_values, float(row[ColNo]))
        
        return column_values



#return Ntot (No. of file) for each channel 
def countFile(HV, Vth, Vamp):
    ##Check Eventno in the dataset
    NtotCh1 = 0
    NtotCh2 = 0
    Ntot = 0
    
    print("\nFor progress bar")
    print("---------------------------")

    #Counting number of events
    print("#Counting no. of events")
    for filename in FilenameList:
        if filename.endswith('.csv') and "Ch1" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename and str(str(Vamp) + "Vamp") in filename:
            NtotCh1 += 1
        if filename.endswith('.csv') and "Ch2" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename and str(str(Vamp) + "Vamp") in filename:
            NtotCh2 += 1
    print("Ch1 EventNo: {}".format(NtotCh1))       
    print("Ch2 EventNo: {}".format(NtotCh2)) 


    #Terminate the program and print error message if the EventNo not match
    if NtotCh1 == NtotCh2:
        Ntot = NtotCh1
    else:
        print("---------------------------")
        print("-------------------------------------")
        sys.exit("EventNo of Ch1 and Ch2 does not match. Please check the files in directory")

    print("---------------------------\n")

    #return No. of file
    return Ntot




def Thershold_Checking(filename):
    # Open the CSV file and read the specified column
    Path = os.path.join(directory_path, filename)
    Ch1column_values = read_csv(Path, 4)
    Path = os.path.join(directory_path, filename[:-5]+"2.csv")
    Ch2column_values = read_csv(Path, 4)

    IsCh1Passed = False
    IsCh2Passed = False
    ThersholdCheckPassed = False

    for value in Ch1column_values:
        if value > 0.6:
            IsCh1Passed = True
            break
    for value in Ch2column_values:
        if value < -0.4:
            IsCh2Passed = True
            break
    if IsCh1Passed and IsCh2Passed:
        ThersholdCheckPassed = True

    return ThersholdCheckPassed





    


#Output analyzed parameter - Leading Time, Trailing Time, Time Width, PeakNo
#channel_column_values for Ch2 need input the flipped one
def AnaylsisOneSignal(filename):

    # Open the CSV file and read the specified column
    Path = os.path.join(directory_path, filename)
    time_column_values = read_csv(Path,3)
    channel_column_values = read_csv(Path,4)

    #initialize all list and parameter we want
    Leading_Time_list = np.array([])
    Trailing_Time_list = np.array([])
    FinalLeadingTime = 0
    FinalTrailingTime = 0
    TimeWidth = 0
    PeakNo = 0
      
    #Get the leading and trailing time by half maximum
    half_max_num = max(channel_column_values) / 2
    shifted_channel_column_values = channel_column_values - half_max_num
                    
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
                        #print("\n")
                        #print(filename)
                        #print("zero! leading have!")
                        break

                    if CurrentValue > 0 and shifted_channel_column_values[j] < 0:
                        Trailing_Time = TimeforZero/TimeforZero_count
                        Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)
                        #print("zero! trailing have!")
                        break
                                    
                    if CurrentValue > 0 and shifted_channel_column_values[j] > 0:
                        Trailing_Time = time_column_values[i]
                        Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)                                       
                        Leading_Time = time_column_values[j]
                        Leading_Time_list = np.append(Leading_Time_list, Leading_Time)
                        #print("zero! trailing have!")
                        #print("\n")
                        #print(filename)
                        #print("zero! leading have!")
                        break

                    if CurrentValue < 0 and shifted_channel_column_values[j] < 0:
                        Leading_Time = time_column_values[i]
                        Leading_Time_list = np.append(Leading_Time_list, Leading_Time) 
                        Trailing_Time = time_column_values[j]
                        Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)                                       
                        print("\n")
                        #print(filename)
                        #print("zero! leading have!")
                        #print("zero! trailing have!")
                        break
                                
        #for simple cases two point is crossing the zero line (HM)
        if CurrentValue < 0 and NextValue > 0:
            Leading_Time = (time_column_values[i]+time_column_values[i+1])/2 
            Leading_Time_list = np.append(Leading_Time_list, Leading_Time)
            #print("\n")
            #print(filename)
            #print("leading have!")

        if CurrentValue > 0 and NextValue < 0:
            Trailing_Time = (time_column_values[i]+time_column_values[i+1])/2 
            Trailing_Time_list = np.append(Trailing_Time_list, Trailing_Time)
            #print("trailing have!")
                    
    print("Leading Time:{}".format(Leading_Time_list))
    print("Trailing Time:{}".format(Trailing_Time_list))


    #PeakNo.
    PeakNo = len(Leading_Time_list)

    #Final LeadingTime, TrailingTime and Timewidth
    if PeakNo == 1:
        FinalLeadingTime = Leading_Time_list[0]
        FinalTrailingTime = Trailing_Time_list[0]
        TimeWidth = FinalTrailingTime - FinalLeadingTime
    if PeakNo > 1:
        FinalLeadingTime = Leading_Time_list[0]
        FinalTrailingTime = Trailing_Time_list[-1]        
        TimeWidth = FinalTrailingTime - FinalLeadingTime
    


    return FinalLeadingTime, FinalTrailingTime, TimeWidth, PeakNo



#output time interval between two signal
def Timeinterval(Ch1FinalLeadingTime, Ch2FinalLeadingTime):
    Timeinterval = Ch2FinalLeadingTime - Ch1FinalLeadingTime
    return Timeinterval



#Return True/False for a specific Working point (Thershold,LT,TT,TW,PNo,TI)
def WorkingPointCheck(filename):
    """
     if Thershold_Checking(filename):
        Ch1LeadingTime, Ch1TrailingTime, Ch1TimeWidth, Ch1PeakNo = AnaylsisOneSignal(filename)
        Ch2LeadingTime, Ch2TrailingTime, Ch2TimeWidth, Ch2PeakNo = AnaylsisOneSignal(filename[:-5]+"2.csv")
        Time_interval = Timeinterval(Ch1LeadingTime, Ch2LeadingTime)

        if Ch1TimeWidth < 1:
            return True
        else:
            return False
    else:
        return False   
    """
    return Thershold_Checking(filename)
    


    

    




#Set Desired Voltage
Vth = 2.2
Vamp = 1.5
print("-------------------------------------")
print("dataset Vth = {}".format(Vth))
print("Please input HV (KV):")
HV = input()




#Initialize a counter for the number of files that meet the criteria
Ch1MuonCount = 0
Ch1FileCount = 0
Ch2MuonCount = 0
Ch2FileCount = 0
Ninterval = 100
Ntot = countFile(HV, Vth, Vamp)



#Word
print("--------------Working----------------")
print("HV:{}kV".format(HV))
print("Scanning {} events in the directory for CH1".format(Ntot))



# Loop through all files in the directory
for filename in FilenameList:
    if filename.endswith('.csv') and "Ch1" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename:
        
        # Counting Muon detected by RPC
        if WorkingPointCheck(filename):
            Ch1MuonCount += 1
        
        # Progress counter
        Ch1FileCount += 1
        
        if Ch1FileCount % Ninterval == 0:
            print("Ch1 progress:{}/{}".format(Ch1FileCount,Ntot))
        

                
                

#Word                
print("\nScanning {} events in the directory for CH2".format(Ntot))

# Loop through all files in the directory
for filename in FilenameList:
    if filename.endswith('.csv') and "Ch2" in filename and str(str(HV)+"KHV") in filename and str(str(Vth) + "Vth") in filename:    
        
        Path = os.path.join(directory_path, filename)
        Ch2column_values = read_csv(Path, 4)

        for value in Ch2column_values:
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
 