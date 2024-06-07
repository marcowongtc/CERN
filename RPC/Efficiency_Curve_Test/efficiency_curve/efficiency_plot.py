import numpy as np
import matplotlib.pyplot as plt

#HVlist
HV_list = []
HV = 4.8
while HV < 6.0:
    HV_list.append(str(HV))
    HV += 0.1
print("HV list: {}".format(HV_list))


#input desirable np.array dataset for plotting
print("Please input desirable dataset (in the form of list)") 
print("Vth:")
Vth_list = input()
print("Vamp:")
Vamp_list = input()

# open the file in read binary mode
eff_file = open("eff_{}Vth_{}Vamp".format(Vth_list, Vamp_list), "rb")
err_file = open("err_{}Vth_{}Vamp".format(Vth_list, Vamp_list),"rb")
eff_array = np.load(eff_file)
err_array = np.load(err_file)

#for testing
#print("eff_{}Vth_{}Vamp".format(Vth_list, Vamp_list))
#print(Vth_list)
#print(Vamp_list)
#print(len(Vth_list))
#print(len(Vamp_list))

#Plotting a effciency plot (x:HV, y:Efficiency)
if len(Vth_list) == 1 and len(Vamp_list) == 1:
    plt.errorbar(HV_list, eff_array, yerr=err_array, linestyle='-', fmt='o', capsize=5, label='Data with Error Bars')
    plt.xlim([float(HV_list[0]), float(HV_list[-1])]) #HVmin, HVmax
    plt.ylim([0, 1])
    plt.xlabel('High Voltage')
    plt.ylabel('efficiency')
    plt.title('efficiency against HV - {}Vth {}Vamp'.format(Vth_list, Vamp_list))
    plt.savefig('Efficiency_HV_Plot_{}Vth_{}Vamp.png'.format(Vth_list, Vamp_list))

#Plotting multiple effciency plot diff Vth
if len(Vth_list) > 1:
    i = 0
    for Vth in Vth_list:
        plt.errorbar(HV_list, eff_array[i], yerr=err_array[i], linestyle='-', fmt='o', capsize=5, label='Vth =' + str(Vth))
        i += 1

    plt.xlim([float(HV_list[0]), float(HV_list[-1])]) #HVmin, HVmax
    plt.ylim([0, 1])

    # Naming the x-axis, y-axis and the whole graph
    plt.xlabel('High Voltage')
    plt.ylabel('efficiency')
    plt.title('efficiency against HV - {}Vth {}Vamp'.format(Vth_list, Vamp_list))
    

    # Adding legend, which helps us recognize the curve according to it's color
    plt.legend()
    plt.savefig('Efficiency_HV_Plot_{}Vth_{}Vamp.png'.format(Vth_list, Vamp_list))


#Plotting multiple effciency plot diff Vamp
if len(Vamp_list) > 1:
    i = 0
    for Vamp in Vamp_list:
        plt.errorbar(HV_list, eff_array[i], yerr=err_array[i], linestyle='-', fmt='o', capsize=5, label='Vamp =' + str(Vamp))
        i += 1

    plt.xlim([float(HV_list[0]), float(HV_list[-1])]) #HVmin, HVmax
    plt.ylim([0, 1])

    # Naming the x-axis, y-axis and the whole graph
    plt.xlabel('High Voltage')
    plt.ylabel('efficiency')
    plt.title('efficiency against HV - {}Vth {}Vamp'.format(Vth_list, Vamp_list))
    

    # Adding legend, which helps us recognize the curve according to it's color
    plt.legend()
    plt.savefig('Efficiency_HV_Plot_{}Vth_{}Vamp.png'.format(Vth_list, Vamp_list))