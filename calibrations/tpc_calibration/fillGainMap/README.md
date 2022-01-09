# TPC Gain & IB maps generation macro.
This macro is generating maps of Gain and IBF for the sPHENIX TPC.

## Input.
As an input the macro uses calibration measurements from Yele University for two modules IROC 35 & 38:
- IROC 35

<p float="left">
  <img src="./Plots/h_map_anode_iroc_35.png" width="300" />
  <img src="./Plots/h_map_measuredibf_iroc_35.png" width="300" /> 
</p>

- IROC 38

<p float="left">
  <img src="./Plots/h_map_anode_iroc_38.png" width="300" />
  <img src="./Plots/h_map_measuredibf_iroc_38.png" width="300" /> 
</p>

## Output.
The macro writes the file *./Files/IBF_Map.root* with the "fake" maps for sPHENIX TPC readout:
<p float="left">
  <img src="./Plots/h_map_modules_anode.png" width="250" />
  <img src="./Plots/h_map_modules_measuredibf.png" width="250" /> 
</p>

## Running macro.

- Setup local environment: ```source /opt/sphenix/core/bin/sphenix_setup.sh -n```
- Run in the batch mode: ```./makeMap.py -b```