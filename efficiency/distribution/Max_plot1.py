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
    MaxNum_Time_list = np.loadtxt("Ch1max_{}HV_{}Vth_{}Vamp.txt".format(HV, Vth, Vamp), delimiter=',')
    Hist_Plot(MaxNum_Time_list)
    plt.title('Histogram of time for max value - {}HV {}Vth {}Vamp'.format(HV, Vth, Vamp))
    plt.savefig("Ch1time_maximum_{}HV_{}Vth_{}Vamp.png".format(HV,Vth,Vamp))
    print("Ch1time_maximum_{}HV_{}Vth_{}Vamp.png is saved!".format(HV,Vth,Vamp))

    #Clear Plot
    plt.clf()



# open the file in read binary mode - Accumulated maxNum_time
MaxNum_Time_Accumlist = np.loadtxt("Ch1maxAccum_{}Vth_{}Vamp.txt".format(HV, Vth, Vamp), delimiter=',')
Hist_Plot(MaxNum_Time_Accumlist)
plt.title('Accumulative Histogram of time for max value for all HV - {}Vth {}Vamp'.format(Vth, Vamp))
plt.savefig("Ch1time_maximumAccum_{}Vth_{}Vamp.png".format(Vth,Vamp))
print("Ch1time_maximumAccum_{}Vth_{}Vamp.png is saved!".format(Vth,Vamp))

#Clear Plot
plt.clf()

