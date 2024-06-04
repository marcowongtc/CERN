import numpy as np
import matplotlib.pyplot as plt

Vth = 2.2
Vamp = 1.5

# open the file in read binary mode - Accumulated maxNum_time
Leading_Time_Accumlist = np.loadtxt("Ch1AccumLeadingTime_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')

#Leading Time Plot
plt.hist(Leading_Time_Accumlist,500)
plt.title('Histogram of Leading Time')
plt.xlabel('Leading Time (s)')
plt.ylabel('Count')
plt.xlim([-3*10**(-7) , -2.2*10**(-7)])
plt.savefig("efficiency_leading_time.png")
plt.clf() #clear plot
