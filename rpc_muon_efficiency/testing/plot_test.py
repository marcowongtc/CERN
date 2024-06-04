import numpy as np
import matplotlib.pyplot as plt


xlist = [0,1,2,3,4,5,6,7]
ylist = [0,1,2,3,4,5,6,7]
errlist = [0,0.1,0.2,0.3,0.4,0.5,0.6,0.7]

plt.errorbar(xlist, ylist, yerr=errlist, linestyle='-', fmt='o', capsize=5, label='Data with Error Bars')
plt.xlim([0, 7]) 
plt.ylim([0, 7])
plt.xlabel('x')
plt.ylabel('y')
plt.title('x aginst y')

plt.savefig('xagiansty.jpg')
plt.savefig('xagiansty.png')