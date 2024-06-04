import numpy as np
import matplotlib.pyplot as plt

Vth = 2.2
Vamp = 1.5

# open the file in read binary mode - Accumulated maxNum_time
Interval_Ch1Ch2_Accumlist = np.loadtxt("Interval_Ch1Ch2_Accumlist_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')

#Leading Time Plot
plt.hist(Interval_Ch1Ch2_Accumlist,500)
plt.title('Histogram of Time Interval')
plt.xlabel('Time Interval (s)')
plt.ylabel('Count')
plt.xlim([1.5*10**(-7) , 2.5*10**(-7)])
plt.savefig("efficiency_time_interval.png")
plt.clf() #clear plot
