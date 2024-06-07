## Test Overview
![Test](/Asset/test_list.png)

## Quick Access
### [Calibration Test](./Calibration_Test/README.md)
### [Manchester Decoder Test](./Manchester_Decoder_Test/README.md)
### [Efficiency Curve Test](./Efficiency_Curve_Test/README.md)
### [Signal Quality Test](./Signal_Quality_Test/README.md)

## Directory Hierarchy

```bash
.
├── README.md
│
├── Calibration_Test
│   ├── README.md
│   ├── gas_gap_current_plot.py
│   └── noise_level_plot.py
│
├── Efficiency_Curve_Test
│   ├── README.md
│   ├── efficiency_curve
│   │   ├── 3wp_plot.py
│   │   ├── count.py
│   │   ├── efficiency_HV_LV_looper.py
│   │   ├── efficiency_HV_looper.py
│   │   ├── efficiency_oneHV.py
│   │   ├── efficiency_oneHV_upgrade.py
│   │   └── efficiency_plot.py
│   └── testing
│       ├── Debug_multipleplot.py
│       ├── histogram.py
│       ├── multiple_plot.py
│       ├── plot_test.py
│       └── save_load_test.py
│
├── Manchester_Decoder_Test
│   ├── Main.C
│   ├── README.md
│   ├── analysis.py
│   ├── csv_to_root.C
│   ├── draw.C
│   ├── example_picture
│   │   ├── bit_size.png
│   │   ├── clock.png
│   │   ├── discriminator_signal.png
│   │   ├── injected_signal.png
│   │   └── signal_word.png
│   └── filenames_to_txt.py
│
├── Signal_Quality_Test
│   ├── File_plot.py
│   ├── Max_distribution1.py
│   ├── Max_distribution2.py
│   ├── Max_plot1.py
│   ├── Max_plot2.py
│   ├── PeakNo_plot1.py
│   ├── PeakNo_plot2.py
│   ├── README.md
│   ├── Report_Plot
│   │   ├── Efficiency_testset_plot.py
│   │   ├── Signal_plot.py
│   │   ├── leading_time_plot.py
│   │   ├── peak_no_plot.py
│   │   ├── time_distribution_plot.py
│   │   ├── time_interval_plot.py
│   │   ├── time_width_plot.py
│   │   └── trailing_time_plot.py
│   ├── Test_Time_plot1.py
│   ├── Time_distribution1.py
│   ├── Time_distribution2.py
│   ├── Time_interval_Ch1Ch2_L-L.py
│   ├── Time_interval_Ch1Ch2_L-T.py
│   ├── Time_interval_plot.py
│   ├── Time_plot1.py
│   └── Time_plot2.py
│
└── Tektronix_for_FE
    ├── Makefile
    ├── README.md
    ├── analysis
    │   ├── Config.h
    │   ├── Maps.h
    │   ├── VarBIS7.h
    │   ├── VarBIS8.h
    │   ├── VarCAEN.h
    │   └── analysis.h
    ├── bin
    │   └── AnalTDCUS
    ├── compilare.txt
    ├── lista_bits.list
    ├── lista_waves.list
    ├── prova.root
    └── util
        └── AnalTDCUS.cxx
```