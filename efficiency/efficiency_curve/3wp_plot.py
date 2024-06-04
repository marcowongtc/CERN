import numpy as np
import matplotlib.pyplot as plt

#HVlist
HV_list = []
HV = 4.8
while HV < 6.0:
    HV_list.append(str(HV))
    HV += 0.1
print("HV list: {}".format(HV_list))


# open the file in read binary mode
eff_file_2 = open("eff_[1.8, 2.2]Vth_[1.3]Vamp", "rb")
err_file_2 = open("err_[1.8, 2.2]Vth_[1.3]Vamp","rb")
eff_array_2 = np.load(eff_file_2)
err_array_2 = np.load(err_file_2)

eff_file = open("eff_[2.2]Vth_[1.5]Vamp", "rb")
err_file = open("err_[2.2]Vth_[1.5]Vamp","rb")
eff_array = np.load(eff_file)
err_array = np.load(err_file)


i = 0
for Vth in [1.8, 2.2]: 
    plt.errorbar(HV_list, eff_array_2[i], yerr=err_array_2[i], linestyle='-', fmt='o', capsize=5, label= str(Vth) +'Vth' + " 1.3Vamp")
    i += 1

plt.errorbar(HV_list, eff_array, yerr=err_array, linestyle='-', fmt='o', capsize=5, label='2.2Vth 1.5Vamp')

plt.xlim([float(HV_list[0]), float(HV_list[-1])]) #HVmin, HVmax
plt.ylim([0, 1])

# Naming the x-axis, y-axis and the whole graph
plt.xlabel('High Voltage (kV)')
plt.ylabel('Efficiency')
plt.title('Efficiency against High Voltage')
    

# Adding legend, which helps us recognize the curve according to it's color
plt.legend()
plt.savefig('Efficiency_HV_Plot_3WP.png')