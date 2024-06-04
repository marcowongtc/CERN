import matplotlib.pyplot as plt


Vth = [1.6, 1.7, 1.8, 1.9, 2.0]
Rate_Vth = [0.00006351626016, 0.0001097099039, 0.0001270325203, 0.00009816149298, 0.0001212583149]

Vamp = [1.1, 1.2, 1.3, 1.4, 1.5]
Rate_Vamp = [0.00008661308204,0.00008083887657,0.0001270325203,0.00009238728751,0.0001270325203]


plt.scatter(Vth, Rate_Vth, color='red', marker='o')
plt.xlabel('Vth (V)')
plt.ylabel('Rate per area (Hzcm^-2)')
plt.ylim([0, 0.0002])
plt.title('Plot of Noise Rate per area against Vth')
plt.savefig("noise_Vth_result.png")
plt.clf()

plt.scatter(Vamp, Rate_Vamp, color='red', marker='o')
plt.xlabel('Vamp (V)')
plt.ylabel('Rate per area (Hzcm^-2)')
plt.ylim([0, 0.0002])
plt.title('Plot of Noise Rate per area against Vamp')
plt.savefig("noise_Vamp_result.png")
plt.clf()
