import os
import csv
import matplotlib.pyplot as plt

#define a fucntion to read csv file and append the value of a column into a list
def read_csv(csv_file, n):
    
    with open(csv_file, 'r') as csv_file: #with open: will help u close the file as u finish
        csv_reader = csv.reader(csv_file)
        column_values = []

        for row in csv_reader:
            column_values.append(float(row[n]))
        
        return column_values


directory_path = '/afs/cern.ch/user/t/tswong/efficiency_plot/dataset_1_8_23_efficiency/'
filename1 = '4.8KHV_2.2Vth_1.5Vamp_efficiency223Wfm_Ch1.csv'
filename2 = '4.8KHV_2.2Vth_1.5Vamp_efficiency223Wfm_Ch2.csv'

#filename1 = '4.8KHV_2.2Vth_1.5Vamp_efficiency216Wfm_Ch1.csv'
#filename2 = '4.8KHV_2.2Vth_1.5Vamp_efficiency216Wfm_Ch2.csv'
#filename = '4.8KHV_2.2Vth_1.5Vamp_efficiency222Wfm_Ch1.csv'
#filename1 = '4.8KHV_2.2Vth_1.5Vamp_efficiency24Wfm_Ch1.csv'

# Open the CSV file and read the specified column
Path1 = os.path.join(directory_path, filename1)
time_value1 = read_csv(Path1, 3)
amplitude_value1 = read_csv(Path1,4)

Path2 = os.path.join(directory_path, filename2)
time_value2 = read_csv(Path2, 3)
amplitude_value2 = read_csv(Path2, 4)

#Signal Plot
plt.plot(time_value1, amplitude_value1, linestyle='-')
plt.plot(time_value2, amplitude_value2, linestyle='-')
#plt.scatter(time_value, amplitude_values, color='red', marker='o')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.xlim([-3*10**(-7), 0.8*10**(-7)])
plt.ylim([-1, 1.5])
plt.title('Discriminator Signal & Trigger Signal')
#plt.title('Discriminator Signal')
plt.savefig("efficiency_program_example5.png")
