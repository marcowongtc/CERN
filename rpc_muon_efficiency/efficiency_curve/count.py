import os
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt

def get_files_with_substring(substring1, substring2):
    current_directory = os.getcwd()
    all_files = os.listdir(current_directory)
    files_with_substring = [filename for filename in all_files if substring1 in filename and substring2 in filename]
    return files_with_substring

def read_csv(csv_file):
    column_index = 4  # Index starts from 0, so 4 represents the fifth column
    fifth_column_values = []
    with open(csv_file, 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            if len(row) > column_index:
                fifth_column_values.append(float(row[column_index]))
    return fifth_column_values


#def main():
    #if len(sys.argv) < 2:
    #    print("Please provide the desired substring as an argument.")
    #    return

    #substring = sys.argv[1]
    
voltage = ["4.8","4.9","5.0","5.1","5.2","5.3","5.4","5.5","5.6","5.7","5.8","5.9","6.0"]
#voltage = ["4.8","4.9"]
eff = []
err = []


for v in voltage:

    substring = v
    print "voltage: ", v, " kV"

    ch1_files = get_files_with_substring(substring, "Ch1")
    ch2_files = get_files_with_substring(substring, "Ch2")

    n_mu_ch1=0
    for ch1 in ch1_files:
    
        csv_file = ch1  
        line_values = read_csv(csv_file)
        for value in line_values:
            if value > 0.6:
                n_mu_ch1 += 1
                break
            
    print "ch1:", n_mu_ch1
    
    n_mu_ch2=0
    for ch2 in ch2_files:
    
        csv_file = ch2  
        line_values = read_csv(csv_file)
        for value in line_values:
            if value < -0.4:
                n_mu_ch2 += 1
                break
             
    print "ch2:", n_mu_ch2
    
    effi = float(n_mu_ch1) / n_mu_ch2
    eff.append(effi)
    print "efficiency: ", effi
    
    errors = np.sqrt(effi*(1-effi)/n_mu_ch2)
    err.append(errors)
    print "Stat Error: ", errors

plt.errorbar(voltage, eff, yerr=err, linestyle='-', fmt='o', capsize=5, label='Data with Error Bars')

plt.xlabel('voltage')
plt.ylabel('efficiency')
plt.title('efficiency 0728')

plt.show()


    
#if __name__ == "__main__":
#    main()
