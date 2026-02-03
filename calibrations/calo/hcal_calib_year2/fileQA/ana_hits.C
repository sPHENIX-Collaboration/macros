#include <TFile.h>
#include <TH1.h>
#include <TList.h>
#include <TProfile2D.h>
#include <TSystemDirectory.h>

#include <format>
#include <fstream>
#include <iostream>
#include <string>

float getTempRun(int /*runnumber*/);

TProfile2D* pr2d_temp = new TProfile2D("pr2d_temp","",24,0,24,64,0,64,15.0,30.0,"s");

void ana_hits() {
    //std::string parantDir = "../output_cosmic_tsc_temp/";
    std::string parantDir = "../cos_only_tsc_spec_output//";
    TSystemDirectory dir(parantDir.c_str(),parantDir.c_str()); // Adjust the directory path
    TList *files = dir.GetListOfFiles();

    if (!files) {
        std::cerr << "No files found in the directory" << std::endl;
        return;
    }

    std::vector<TString> fileNames;
    std::vector<int> runNumbers;

    // Collect the file names and run numbers
    TIter next(files);
    TObject *fileObj;
    while ((fileObj = next())) {
        TString fileName = fileObj->GetName();
        // Only add files that match the pattern (out_<RUNNUMBER>.root)
        if (fileName.Contains("out_") && fileName.EndsWith(".root")) {
            fileNames.push_back(fileName);

            // Extract the run number (assumes the filename format is out_<RUNNUMBER>.root)
            TString runStr = fileName(4, fileName.Index(".root") - 4); // Extract the number after 'out_'
            int runNumber = runStr.Atoi();
            runNumbers.push_back(runNumber);
        }
    }

    // Declare a histogram with the number of bins equal to the number of files
    int numFiles = fileNames.size();
    if (numFiles == 0) {
        std::cerr << "No matching files found" << std::endl;
        return;
    }

    //////////////////////////////////////////
    //  Make run list
    ///////////////////////////////////////////
    std::ofstream outFile("runList.txt");

    // Check if the file is open
    if (!outFile.is_open()) {
        std::cerr << "Unable to open the file for writing!" << std::endl;
        return ; // Return error if the file could not be opened
    }

    // Loop over the vector and write each run number to the file
    for (const int& runNumber : runNumbers) {
        outFile << runNumber << std::endl; // Write each run number on a new line
    }

    // Close the file after writing
    outFile.close();


    /////////////////////////////////////////
    //  analyze rooot files
    //////////////////////////////////////////

    TH1 *entryCountHist = new TH1F("h_hits", "Entries in h", numFiles, 0, numFiles);
    TH1 *h_events_run =  new TH1F("h_events_run", "Entries in h", numFiles, 0, numFiles);
    //TH1 *h_towerHits_run =  new TH1F("h_towerHits_run", "Entries in h", numFiles, 0, numFiles);
    TH1 *runNumberHist = new TH1F("runNumberHist", "Run Numbers for Each File", numFiles, 0, numFiles);
    TH1 *h_temp_run = new TH1F("h_temp_run", "", numFiles, 0, numFiles);

    // Loop over all the files
    for (int i = 0; i < numFiles; ++i) {
        TString fileName = parantDir.c_str()  + fileNames[i];
        TFile *file = TFile::Open(fileName);

        if (file && !file->IsZombie()) {
	  TH1 *h {nullptr};
	  file->GetObject("hcalout_eta_20",h);
	  TH1 *h_event {nullptr};
	  file->GetObject("h_event",h_event);

            if (h) {
                float eLow = 1;
                float eHigh= 5;
                int entries = h->Integral(h->FindBin(eLow),h->FindBin(eHigh));
                entryCountHist->SetBinContent(i + 1, entries); // Set the number of entries in the corresponding bin
                h_events_run->SetBinContent(i+1,h_event->GetEntries());
                runNumberHist->SetBinContent(i + 1, runNumbers[i]); // Set the run number in the corresponding bin

                std::cout << "File: " << fileName << " has " <<  h_event->GetEntries() << " events" << std::endl;
                float temp = getTempRun(runNumbers[i]);
                 h_temp_run->SetBinContent(i+1,temp);
            } else {
                std::cout << "Histogram 'h' not found in file " << fileName << std::endl;
            }
            file->Close();
        } else {
            std::cout << "File " << fileName << " not found or unable to open" << std::endl;
        }
    }
    TFile *outputFile = new TFile("output/anaOut.root", "RECREATE");
    entryCountHist->Write();
    h_events_run->Write();
    runNumberHist->Write();
    h_temp_run->Write();
    pr2d_temp->Write();


    outputFile->Close();
    std::cout << "Histogram saved to entry_count_histograms.root" << std::endl;
}



float getTempRun(int runnumber){

  TFile* f = new TFile(std::format("../tempHists/ohcal_temp_hist_{}.root",runnumber).c_str());
  if (!f){
     std::cout << "could not find temp file" << std::endl;
     return 0;

    }


  TH2* h {nullptr};
  f->GetObject("h_ohcal_temp",h);

  if (!h){
     std::cout << "no temp hist" << std::endl;
     return 0;

   }

  float temp = 0;
  int tempC = 0;
  for(int ix=1; ix<h->GetXaxis()->GetNbins()+1; ix++){
    for(int iy=1;iy<h->GetYaxis()->GetNbins()+1; iy++){
        float t = h->GetBinContent(ix,iy);
       pr2d_temp->Fill(ix-1,iy-1,t);
       if (t > 15 && t < 30){
          temp += t;
          tempC++;
       }
    }
  }

  delete h;
  delete f;
  if (tempC < 3) return 0;
  return temp/tempC;    

}
