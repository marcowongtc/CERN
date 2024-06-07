import matplotlib.pyplot as plt


x = [500,1000,1500,2000,2500,3000,3500,4000,4500,4600,4700,4800,4900,5000,5100,5200,5300,5400,5500,5600,5700,5800,5900,6000]
y = [0.003, 0.006, 0.01, 0.013, 0.016, 0.02, 0.024, 0.03, 0.036, 0.0375, 0.041, 0.041, 0.045, 0.05, 0.054, 0.058, 0.064, 0.07, 0.077, 0.092, 0.103, 0.121, 0.14, 0.17]


plt.plot(x, y, linestyle='-')
plt.scatter(x, y, color='red', marker='o')
plt.xlabel('High Voltage (V)')
plt.ylabel('Gas Gap Current (micro A)')
plt.ylim([0, 0.2])
plt.title('Graph of Gas Gap Current agianst High Voltage')
plt.savefig("gas_gap_current_result.png")
