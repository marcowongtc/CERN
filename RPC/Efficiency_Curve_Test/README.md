## Efficiency Curve Test
### System
![Trigger System](/Asset/scintillator_trigger_system.png)

***

### Signal Detection & Efficiency Calculation
Here we measure the efficiency curve of the RPC Muon Spectrometer by Counting the number of muon detected by the scintillator and the newly upgraded RPC simultaneously and number of muon missed by RPC.

![Signal Detection](/Asset/signal_detection.png)

The number of muon is obtained by counting the detector signals with exceeding a threshold voltage $V_{th}$


![Efficiency Calculation](/Asset/efficiency_calculation.png)
> #### Efficiency ε
>$ε = N_{coinc} / (N_{coinc} + N_{miss})$
> #### Error Bar δ
>$δ = \sqrt{ε(1-ε)/(N_{coinc} + N_{miss})}$

***

### Efficiency Curve Plotting

Efficiency is plotted against the High Voltage applied HV with three working points `3WP`
>- to uncover possible defects in RPC
>- to learn the RPC science | avalanche, electronics, detector physics  

![Efficiency Curve](/Asset/Efficiency_3WP.png)

***

## Cont: [Signal Quality Test](/RPC/Signal_Quality_Test/) | Further investigation on Efficiency Algorithm  

Quality of Peaks is analyzed with several notable characteristic 
> - Peak Maximum
> - Time Width
> - Leading Time + Trailing Time 
> - Peak Number 

## Notes 
[Learn | Scintillator Trigger System with Oscilloscope + Acceptance & Efficiency Curve](https://shaded-cannon-4d7.notion.site/Learn-Scintillator-Trigger-System-with-Oscilloscope-Acceptance-Efficiency-Curve-f71d957bbe9442a3851bbb81d372d609?pvs=4)

[Test | Efficiency Curve of detector vs Quality of Detector](https://shaded-cannon-4d7.notion.site/Test-Efficiency-Curve-of-detector-vs-Quality-of-Detector-bb91359c61794fc68976e825da494a1f?pvs=4)

[Code | Efficiency Plot with trigger system and detector](https://shaded-cannon-4d7.notion.site/Code-Efficiency-Plot-with-trigger-system-and-detector-5355bde4fc2348618c47cd9c6af83cb1?pvs=4)


