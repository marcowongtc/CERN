# Machester decoding



## Introduction

The package is to decode the output words from an oscilloscope, which are manchester encoded. The Manchester encoding will be used in the new front-end electronics of the RPC (resistive plate chamber) of the muon spectrometer.

## Setup
In Lxplus, do the following steps:
```
setupATLAS
lsetup git
git clone https://:@gitlab.cern.ch:8443/kfung/rpc_upgrade_manchester.git
cd rpc_upgrade_manchester
```
Place all the CSV files of one dataset in a directory and put it in the `rpc_upgrade_manchester` directory.

## Read the CSV files and merge them into one ROOT file
Run `filenames_to_txt.py` to generate `csvFileList.txt` containing all the names of the CSV files:

```
cd <csv_directory>    #Change to the directory containing the CSV files
python ../filenames_to_txt.py
```

Modify `csv_to_root.C` if necessary. (The "TIME" column should be always in the CSV files, but the different channels may not. Modify **vector\<string\> read_channel** to read only the available channels)

Inside `<csv_directory>`, run

```
root -l -q ../csv_to_root.C
```
A ROOT file `merged_data.root` will be produced. Check if the necessary variables are in the file by
```
root -l merged_data.root
root [1] tree->Print()
```

## Decode

Adjust the setting in the main function in `Main.C`:
* **string merged_file_name** and **string treename**: The names of the input data file and its tree (which are set by `csv_to_root.C`)
* **tree->SetBranchAddress("XXX", &XXX_ch)** etc: The channel names -- decode_ch is the channel containing the encoded word, injected_signal_ch is the channel for the injected signal (![see here](https://gitlab.cern.ch/kfung/rpc_upgrade_manchester/-/blob/master/example_picture/injected_signal.png)), clock_ch is the channel for the data clock, and disc_signal_ch is the channel for the discriminator signal (![see here](https://gitlab.cern.ch/kfung/rpc_upgrade_manchester/-/blob/master/example_picture/discriminator_signal.png)). The names are simply "CH1", "CH2", "CH3" and "CH4" (as defined in `csv_to_root.C`), but the order is subject to change.

The decode() function is the major function for decoding the words. It takes in the following parameters:
* **TTree \*tree**, **int& event_number**: the variables for the tree and event number defined previously. Usually you don't have to change these
* **vector\<float\>\* clocktime**: the variable for the time. Usually it is also safe to keep it unchanged
* **vector\<vector\<float\>\*\> decode_chs**: the vector for the decoding channels. Note that it is a vector, so it's like {decode_ch}. This enables us to decode more than one available decoding channels, e.g. {decode_ch_1, decode_ch_2}. (But in this case it is suggested to simply split it into two lines for separate channels. This option is kept only for easier testing of the code)
* **vector\<float\>\* injected_signal_ch**, **vector\<float\>\* disc_signal_ch**: the channels for the injected signal and discriminator signal. If they are not available, simply put "nullptr".
* **const string& toWrite_output_name**: the output ROOT file name
* **const float& start_ns**, **const float& end_ns**: the starting time and ending time for the channels in nanosecond
* **vector\<vector\<float\>\*\> clock_chs={}**: the vector for the channels for finding the positions of the reset. Again, it is a vector, corresponding to **vector\<vector\<float\>\*\> decode_chs**. The channels can be the data clock, or they can be the decoding channels itself if no data clock is available
* **bool fix_bit_size**: setting this to true will fix the bit size for all events
* **bool writeFile**: setting this to true will generate the output root file
* **bool writeScreenOutput**: setting this to true will store the screen output to a text file, otherwise it will print to the screen
* **const float& disc_signal_threshold**, **const float& injected_signal_threshold**: the voltage thresholds for locating the signal edges

To run the main code, go to `<csv_directory>` and run
```
root -l -q ../Main.C
```
An output root file will be produced. Check if the necessary variables are in the file by
```
root -l <decode output ROOT filename>
root [1] tree->Print()
```

The screen output will also be saved to a txt file (if setting "writeScreenOutput=true"). The decode information of every event is recorded.

## Decode logic
The main code for the decoding is the `decode()` function in `Main.C`. The logic is to locate the transitions separated by one bit size (~3-4ns).

First, the bit size will be calculated. This is done by calculating the average period of the oscillation in the region where no transmission is occured. (![see here](https://gitlab.cern.ch/kfung/rpc_upgrade_manchester/-/blob/master/example_picture/bit_size.png))

Then, if the data clock is available, it will be used to determine the location where the reset occurs (each separated by 50 ns). (![see here](https://gitlab.cern.ch/kfung/rpc_upgrade_manchester/-/blob/master/example_picture/clock.png)) The signature of a reset is two consecutive local minima. The duration of the reset is also calculated. If the clock is not available, the code will estimate the reset duration by aligning the end of the last bit of the word to the start of the "no-signal region".

The start of the transmission is located by finding the Manchester error, which contains a much longer zero which cannot be seen in any other regions. The transition is found by adding one bit size to the previous transition. An up-to-down transition is decoded as "0", while a down-to-up transition is decoded as "1". (![see here](https://gitlab.cern.ch/kfung/rpc_upgrade_manchester/-/blob/master/example_picture/signal_word.png))

## Other useful files
`draw.C` is a simple code to draw the difference channels for visualization. The starting, ending time, channel and event number to be drawn can be set in the arguments. If you want to draw some vertical lines (e.g. the time of each transition) for easier visualization, you can invlude the times in **vector\<float\> draw_red_vertical** and **vector\<float\> draw_green_vertical**. It can be easily run in `<csv_directory>` by
```
root -l ../draw.C
```
or set the arguments directly in the command line:
```
root -l '../draw.C(-50, 50, "CH1", 0)'
```

`analysis.py` is a simple python script to draw different distributions to pdf files. Open the script, set the file names and the **actions** list, then run it in `<csv_directory>` by
```
python ../analysis.py
```
