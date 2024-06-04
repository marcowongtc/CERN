import numpy as np
import matplotlib.pyplot as plt

list1 = np.array([-1,-1,3,3,3,3,3,3,2,2,1,3,2,2,2,4,4,4,5,5])
list2 = np.array([-1,-1,3,3,3,3,3,3,2,2,1,3,2,2,2,4,4,4,5,5]) + 5

def Hist_plot(list):
    #plot histogram
    #setting BinNo (Freedman Diaconis number of bins)
    q25, q75 = np.percentile(list, [25, 75])
    bin_width = 2 * (q75 - q25) * len(list) ** (-1/3)
    bins = round((list.max() - list.min()) / bin_width)
    print("Freedman Diaconis number of bins:", bins)
    print(list)
    plt.hist(list, bins = bins)
    
    plt.show()
    plt.clf()


Hist_plot(list1)
Hist_plot(list2)
