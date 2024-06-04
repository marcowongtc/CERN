import numpy as np
import matplotlib.pyplot as plt

# reshape: wrong to do this x_list.reshape((3,3)), not a function to reshape instead it will output the reshaped array
x_list = np.array([1,2,3])
y_list = np.array([1,2,3,2,3,4,3,4,5]).reshape((3,3))
err_list = np.array([0.1,0.1,0.1,0.2,0.2,0.2,0.3,0.3,0.3]).reshape((3,3))


print(x_list)
print(y_list)
print(y_list[0])

# important for x_list + y_list[i] + errlist[i] match the same dimension 
for i in [0,1,2]: 
    #plt.plot(x_list, y_list[i], label = '{}'.format(i))
    plt.errorbar(x_list, y_list[i], yerr=err_list[i], linestyle='-', fmt='o', capsize=5, label='Vth =' + str(i))


plt.xlim([0, 4]) #HVmin, HVmax
plt.ylim([0, 7])

plt.xlabel('x')
plt.ylabel('y')
plt.title('y aginst x')


plt.legend()
plt.show() 