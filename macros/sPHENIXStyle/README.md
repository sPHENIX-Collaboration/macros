Please find instruction at https://wiki.bnl.gov/sPHENIX/index.php/Plot_template . Here is a snapshot:

# How to use

To ensure some consistency among plots produced by different sPHENIX members, a candidate style file is being proposed (May 2017), based on the ATLAS style file. Please click here for the gzipped tar file.
For general use, copy or link the ```sPHENIXStyle.h``` and ```sPHENIXStyle.C``` files somewhere, either locally or in your macro path (```gROOT->GetMacroPath()``` will tell you, and add it in ```.rootrc``` if you need one).
Then do
```
[0] gROOT->LoadMacro("sPhenixStyle.C") 
[1] SetsPhenixStyle()
[2] .x test_style.C  // give it a try
```
and you're good to go (even better, add this all into your ```.rootlogon.C``` file)

# Rules of the sPHENIX style

* Don't talk about the style file.
* Always use the style file.
* Units should always be indicated in brackets "[ ]", while counts will typically specify the bin width "Events / 2 GeV" or "Events / 0.13 rad".
* Every plot should have a legend indicating "sPHENIX", "sPHENIX Preliminary", "sPHENIX Simulation", etc.
* sPHENIX should be bold, italic
* Where possible, indicate the system being considered and it's energy
* "MC" predictions should be filled, with distinguishable colors
* Data or pseudodata should always be histograms or graphs with error bars
* Elements should never overlap, with each other or with the axis
