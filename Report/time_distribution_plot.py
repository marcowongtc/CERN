import numpy as np
import matplotlib.pyplot as plt



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
Vth = 2.2
Vamp = 1.5



def PeakNo_Plot(histlist):
    print("--------------Working----------------")
    
    #excluding all 1
    histlist = list(filter(lambda x: x != 1, histlist))
    plt.hist(histlist)

    # Naming the x-axis, y-axis and the whole graph
    plt.xlabel('Peak No')
    plt.ylabel('Count')
    plt.xlim([0 , 40])


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

