import numpy as np
import matplotlib.pyplot as plt

Vth = 2.2
Vamp = 1.5

# open the file in read binary mode - Accumulated maxNum_time
Trailing_Time_Accumlist= np.loadtxt("Ch1AccumTrailingTime_{}Vth_{}Vamp.txt".format(Vth, Vamp), delimiter=',')

#Leading Time Plot
plt.hist(Trailing_Time_Accumlist,500)
plt.title('Histogram of Trailing Time')
plt.xlabel('Leading Time (s)')
plt.ylabel('Count')
plt.xlim([-3*10**(-7) , -1.5*10**(-7)])
plt.savefig("efficiency_trailing_time.png")
plt.clf() #clear plot
