# TPC padplane mapping

This repository contains macros to generate TPC padplane geometry

## Reading maps from the padplane design

ChannelMapping.ipynb provides functionality to generate CSV files containing mapping of the pads to the FEEs.

Read files from Eagle drawings:
```
./PadPlane/AutoPad-R3-RevA.sch
./PadPlane/AutoPad-R3-RevA.brd
./PadPlane/AutoPad-R2-RevA-Pads.sch
./PadPlane/AutoPad-R2-RevA-Pads.brd
./PadPlane/AutoPad-R1-RevA.sch
./PadPlane/AutoPad-R1-RevA.brd
```

Generated CSV files:
```
./PadPlane/AutoPad-R3-RevA.sch.ChannelMapping.csv
./PadPlane/AutoPad-R2-RevA-Pads.sch.ChannelMapping.csv
./PadPlane/AutoPad-R1-RevA.sch.ChannelMapping.csv
```

## Creating CDBTTree

CDBTTree-s are used to store detector data, in this case it is TPC padplane electronics to geometry.

To generate CDBTTree from CSV files produced above, do:
```
root -l TestCDBTTreeTPC.C
```