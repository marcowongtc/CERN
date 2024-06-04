import numpy as np
import matplotlib.pyplot as plt

Vth_list = [1.8,2.2]
HV_list = [4.8, 4.9, 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.0]


eff_list = np.array([0.032, 0.036, 0.05, 0.16, 0.28, 0.39, 0.49, 0.57,0.7,0.722, 0.728, 0.776, 0.768, 0.032, 0.05, 0.084,0.176, 0.28,0.48, 0.55,0.63, 0.66, 0.74, 0.73, 0.76, 0.768])
err_list =  np.array([ 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01])




#reshape the arange reshape(n:Vth -row ,m:HV -col)
eff_list = eff_list.reshape(len(Vth_list), len(HV_list))
err_list = err_list.reshape(len(Vth_list), len(HV_list))



#------------------------------------------
##plot Efficiency Plot (x:HV, y:Efficiency)
#------------------------------------------
#Looping plot effi vs HV of diff value of Vth
i = 0
for Vth in Vth_list:
    plt.errorbar(HV_list, eff_list[i], yerr=err_list[i], linestyle='-', fmt='o', capsize=5, label='Vth =' + str(Vth))
    i += 1

plt.xlim([float(HV_list[0]), float(HV_list[-1])]) #HVmin, HVmax
plt.ylim([0, 1])

# Naming the x-axis, y-axis and the whole graph
plt.xlabel('High Voltage')
plt.ylabel('efficiency')
plt.title('efficiency against HV with different Vth')

# Adding legend, which helps us recognize the curve according to it's color
plt.legend()

plt.savefig('Efficiency_HV_LV_Plot.png')