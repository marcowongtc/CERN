import numpy as np
import matplotlib.pyplot as plt

Vth = 2.2
Vamp = 1.5

# open the file in read binary mode - Accumulated maxNum_time
Peak_No_Accumlist = np.loadtxt("Ch1AccumPeakNo_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')

#Leading Time Plot
Peak_No_Accumlist = list(filter(lambda x: x != 1, Peak_No_Accumlist))
plt.hist(Peak_No_Accumlist, 20)
plt.title('Histogram of Peak Number')
plt.xlabel('Peak Number')
plt.ylabel('Count')
plt.xlim([0 , 20])
plt.savefig("efficiency_peak_number.png")
plt.clf() #clear plot
