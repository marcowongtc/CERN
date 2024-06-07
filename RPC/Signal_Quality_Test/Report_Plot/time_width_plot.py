import numpy as np
import matplotlib.pyplot as plt

Vth = 2.2
Vamp = 1.5

# open the file in read binary mode - Accumulated maxNum_time
Time_width_Accumlist = np.loadtxt("Ch1AccumTimeWidth_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')

#Leading Time Plot
plt.hist(Time_width_Accumlist,500)
plt.title('Histogram of Time Width')
plt.xlabel('Time Width (s)')
plt.ylabel('Count')
plt.xlim([0*10**(-7) , 0.7*10**(-7)])
plt.savefig("efficiency_time_width.png")
plt.clf() #clear plot
