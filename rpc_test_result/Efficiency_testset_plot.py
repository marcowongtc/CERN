import matplotlib.pyplot as plt

HV_list = [4.8, 4.9, 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.0]
eff_array = [0.09, 0.05, 0.21, 0.21, 0.5, 0.55, 0.72, 0.72, 0.76, 0.83, 0.83, 0.77, 0.88]
err_array = [0.03, 0.02, 0.04, 0.04, 0.05, 0.05, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 0.03]


plt.errorbar(HV_list, eff_array, yerr=err_array, linestyle='-', fmt='o', capsize=5, label='Data with Error Bars')
plt.xlim([float(HV_list[0]), float(HV_list[-1])]) #HVmin, HVmax
plt.ylim([0, 1])
plt.xlabel('High Voltage (kV)')
plt.ylabel('Efficiency')
plt.title('Efficiency against High Voltage - 2.2Vth 1.5Vamp')
plt.savefig('Efficiency_plot_test.png')