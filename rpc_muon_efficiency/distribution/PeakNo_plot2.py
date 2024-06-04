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

def PeakNo_Plot(histlist):
    print("--------------Working----------------")
    
    #excluding all 1
    histlist = list(filter(lambda x: x != 1, histlist))
    plt.hist(histlist)
    print(histlist)

    # Naming the x-axis, y-axis and the whole graph
    plt.xlabel('Peak No')
    plt.ylabel('Count')



#Set desired Voltage
HV_list = V_GenList(4.8, 6.0, 0.1)
Vth = 2.2
Vamp = 1.5
print("HV list: {}".format(HV_list))
print("Vth: {}".format(Vth))
print("Vamp: {}".format(Vamp))






#Peak No Plot
for HV in HV_list:
    Peak_No_Totlist = np.loadtxt("Ch2PeakNo_{}HV_{}Vth_{}Vamp.txt".format(HV, Vth, Vamp), delimiter=',')
    PeakNo_Plot(Peak_No_Totlist)
    plt.title('Histogram of PeakNo - {}HV {}Vth {}Vamp'.format(HV,Vth, Vamp))
    plt.savefig("Ch2PeakNo_{}HV_{}Vth_{}Vamp.png".format(HV,Vth,Vamp))
    print("Ch2PeakNo_{}HV_{}Vth_{}Vamp.png is saved!".format(HV,Vth,Vamp))
    plt.clf() #clear plot

Peak_No_Accumlist = np.loadtxt("Ch2AccumPeakNo_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')
PeakNo_Plot(Peak_No_Accumlist)
plt.title('Accumulated Histogram of PeakNo - {}Vth {}Vamp'.format(Vth, Vamp))
plt.savefig("Ch2AccumPeakNo_{}Vth_{}Vamp.png".format(Vth,Vamp))
print("Ch2AccumPeakNo_{}Vth_{}Vamp.png is saved!".format(Vth,Vamp))
plt.clf() #clear plot