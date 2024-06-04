import numpy as np
import matplotlib.pyplot as plt

#define a funciton to generate voltage list
def V_GenList(V_min, V_max, V_interval):
    V_list = []
    V = V_min
    while V <= V_max:
        V_list.append(V)
        V += V_interval
    return V_list

#define plotting a histogram (binsize is fit to the normal distribution)
def Hist_Plot(list):
    #plot histogram
    #setting BinNo (Freedman Diaconis number of bins)
    #word
    print("--------------Working----------------")
    q25, q75 = np.percentile(list, [25, 75])
    bin_width = 2 * (q75 - q25) * len(list) ** (-1/3)
    bins = round((list.max() - list.min()) / bin_width)
    print("Freedman Diaconis number of bins:", bins)
    print(list)
    if bin > 5000:
        plt.hist(list, bins = 5000)
    else:
        plt.hist(list, bins = bins)

    # Naming the x-axis, y-axis and the whole graph
    plt.xlabel('Time')
    plt.ylabel('Count')




#Set desired Voltage
HV_list = V_GenList(4.8, 6.0, 0.1)
Vth = 2.2
Vamp = 1.5
print("HV list: {}".format(HV_list))
print("Vth: {}".format(Vth))
print("Vamp: {}".format(Vamp))


for HV in HV_list:
    # open the file in read binary mode - each HV maxNum_time
    Leading_Time_Totlist = np.loadtxt("Ch1LeadingTime_{}HV_{}Vth_{}Vamp.txt".format(HV, Vth, Vamp), delimiter=',')
    Trailing_Time_Totlist= np.loadtxt("Ch1TrailingTime_{}HV_{}Vth_{}Vamp.txt".format(HV, Vth, Vamp), delimiter=',')
    Time_width_Totlist = np.loadtxt("Ch1TimeWidth_{}HV_{}Vth_{}Vamp.txt".format(HV, Vth, Vamp), delimiter=',')

    #Leading Time Plot
    Hist_Plot(Leading_Time_Totlist)
    plt.title('Histogram of time for leading time - {}HV {}Vth {}Vamp'.format(HV, Vth, Vamp))
    plt.savefig("Ch1LeadingTime_{}HV_{}Vth_{}Vamp.png".format(HV,Vth,Vamp))
    print("Ch1LeadingTime_{}HV_{}Vth_{}Vamp.png is saved!".format(HV,Vth,Vamp))
    plt.clf() #clear plot

    #Trailing Time Plot
    Hist_Plot(Trailing_Time_Totlist)
    plt.title('Histogram of time for trailing time - {}HV {}Vth {}Vamp'.format(HV, Vth, Vamp))
    plt.savefig("Ch1TrailingTime_{}HV_{}Vth_{}Vamp.png".format(HV,Vth,Vamp))
    print("Ch1TrailingTime_{}HV_{}Vth_{}Vamp.png is saved!".format(HV,Vth,Vamp))
    plt.clf() #clear plot

    #Time Width Plot
    Hist_Plot(Time_width_Totlist)
    plt.title('Histogram of time for time width - {}HV {}Vth {}Vamp'.format(HV,Vth, Vamp))
    plt.savefig("Ch1TimeWidth_{}HV_{}Vth_{}Vamp.png".format(HV,Vth,Vamp))
    print("Ch1TimeWidth_{}HV_{}Vth_{}Vamp.png is saved!".format(HV,Vth,Vamp))
    plt.clf() #clear plot




# open the file in read binary mode - Accumulated maxNum_time
Leading_Time_Accumlist = np.loadtxt("Ch1AccumLeadingTime_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')
Trailing_Time_Accumlist= np.loadtxt("Ch1AccumTrailingTime_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')
Time_width_Accumlist = np.loadtxt("Ch1AccumTimeWidth_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')

#Leading Time Plot
Hist_Plot(Leading_Time_Accumlist)
plt.title('Histogram of time for leading time - {}Vth {}Vamp'.format(Vth, Vamp))
plt.savefig("Ch1AccumLeadingTime_{}Vth_{}Vamp.png".format(Vth,Vamp))
print("Ch1AccumLeadingTime_{}Vth_{}Vamp.png is saved!".format(Vth,Vamp))
plt.clf() #clear plot

#Trailing Time Plot
Hist_Plot(Trailing_Time_Accumlist)
plt.title('Accumulated Histogram of time for trailing time - {}Vth {}Vamp'.format(Vth, Vamp))
plt.savefig("Ch1AccumTrailingTime_{}Vth_{}Vamp.png".format(Vth,Vamp))
print("Ch1AccumTrailingTime_{}Vth_{}Vamp.png is saved!".format(Vth,Vamp))
plt.clf() #clear plot

#Time Width Plot
Hist_Plot(Time_width_Accumlist)
plt.title('Accumulated Histogram of time for time width - {}Vth {}Vamp'.format(Vth, Vamp))
plt.savefig("Ch1AccumTimeWidth_{}Vth_{}Vamp.png".format(Vth,Vamp))
print("Ch1AccumTimeWidth_{}Vth_{}Vamp.png is saved!".format(Vth,Vamp))
plt.clf() #clear plot

