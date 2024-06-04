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
    Interval_Ch1Ch2_list = np.loadtxt("Interval_Ch1Ch2_{}HV_{}Vth_{}Vamp.txt".format(HV,Vth,Vamp), delimiter=',')
    
    #Time Interval Plot
    Hist_Plot(Interval_Ch1Ch2_list)
    plt.title('Histogram of time interval - {}HV {}Vth {}Vamp'.format(HV,Vth, Vamp))
    plt.savefig("TimeInterval_{}HV_{}Vth_{}Vamp.png".format(HV,Vth,Vamp))
    print("TimeInterval_{}HV_{}Vth_{}Vamp.png is saved!".format(HV,Vth,Vamp))
    plt.clf() #clear plot



# open the file in read binary mode - Accumulated maxNum_time
Interval_Ch1Ch2_Accumlist = np.loadtxt("Interval_Ch1Ch2_Accumlist_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')

#Time Interval Plot
Hist_Plot(Interval_Ch1Ch2_Accumlist)
plt.title('Accumulated Histogram of time interval - {}Vth {}Vamp'.format(Vth, Vamp))
plt.savefig("AccumTimeInterval_{}Vth_{}Vamp.png".format(Vth,Vamp))
print("AccumTimeInterval_{}Vth_{}Vamp.png is saved!".format(Vth,Vamp))
plt.clf() #clear plot

