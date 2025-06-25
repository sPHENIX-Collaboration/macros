#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <functional>
#include <concepts>

// root includes --
#include <TH1.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::make_pair;
using std::vector;
using std::stringstream;

namespace myUtils {
    void setEMCalDim(TH1* hist);

    pair<Int_t, Int_t> getSectorIB(Int_t iphi, Int_t ieta);
    pair<Int_t, Int_t> getSectorIB(Int_t towerIndex);
    vector<string> split(const string &s, const char delimiter);
    TFitResultPtr doGausFit(TH1* hist, Double_t start, Double_t end, const string &name = "fitFunc");

    template<typename Func>
    concept InvocableWithString = std::invocable<Func, const std::string&>;

    template<InvocableWithString Callable>
    bool readCSV(const std::filesystem::path& filePath, Callable lineHandler, bool skipHeader = true);

    Int_t m_nsector = 64;
    Int_t m_nchannel_per_sector = 384;
    Int_t m_nchannel_per_ib = 64;
    Int_t m_nphi = 256;
    Int_t m_neta = 96;
    Int_t m_nib = 384;
    Int_t m_nib_per_sector = 6;
    Int_t m_ntowIBSide = 8;
}

TFitResultPtr myUtils::doGausFit(TH1* hist, Double_t start, Double_t end, const string &name) {
    // fit calib hist
    TF1 *fitFunc = new TF1(name.c_str(), "gaus", start, end);
    Double_t initialAmplitude = hist->GetMaximum();
    Double_t initialMean = hist->GetMean();
    Double_t initialSigma = hist->GetRMS();

    fitFunc->SetParameter(0, initialAmplitude);
    fitFunc->SetParameter(1, initialMean);
    fitFunc->SetParameter(2, initialSigma);

    // You can also set parameter names for better readability in the stats box
    fitFunc->SetParName(0, "Amplitude");
    fitFunc->SetParName(1, "Mean");
    fitFunc->SetParName(2, "Sigma");

    // Set some visual properties for the fit line
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);
    fitFunc->SetLineStyle(kDashed); // Optional: make it dashed

    TFitResultPtr fitResult = hist->Fit(fitFunc, "RS"); // Fit within range, store result, quiet

    if (fitResult.Get()) { // Check if TFitResultPtr is valid
        cout << "\n----------------------------------------------------" << endl;
        cout << "Fit Results for function: " << fitFunc->GetName() << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "Fit Status: " << fitResult->Status() << " (0 means successful)" << endl;
        if (fitResult->IsValid()) { // Check if the fit is valid (e.g., covariance matrix is good)
            cout << "Fit is Valid." << endl;
            for (UInt_t i = 0; i < static_cast<UInt_t>(fitFunc->GetNpar()); ++i) {
                cout << "Parameter " << fitFunc->GetParName(static_cast<Int_t>(i)) << " (" << i << "): "
                          << fitResult->Parameter(i) << " +/- " << fitResult->ParError(i) << endl;
            }
            cout << "Resolution: Sigma/Mean = " << fitResult->Parameter(2) / fitResult->Parameter(1) << endl;
            cout << "Chi^2 / NDF: " << fitResult->Chi2() << " / " << fitResult->Ndf()
                      << " = " << (fitResult->Ndf() > 0 ? fitResult->Chi2() / fitResult->Ndf() : 0) << endl;
            cout << "Probability: " << TMath::Prob(fitResult->Chi2(), static_cast<Int_t>(fitResult->Ndf())) << endl;
        } else {
            cout << "Fit is NOT Valid." << endl;
        }
        cout << "----------------------------------------------------" << endl;
    } else {
        cout << "Fit did not return a valid TFitResultPtr." << endl;
    }

    return fitResult;
}

/**
 * @brief Reads a CSV (or any line-delimited) file and applies a handler function to each line.
 *
 * @tparam Callable The type of the function/lambda to be called for each line.
 * Must be invocable with a 'const std::string&'.
 * @param filePath The path to the input file.
 * @param lineHandler A function, lambda, or functor that takes a 'const std::string&' (the line)
 * and processes it.
 * @param skipHeader If true, the first line of the file will be read and discarded.
 * @return true if the file was successfully opened and read, false otherwise.
 */
template<myUtils::InvocableWithString Callable> // Using the more general concept for wider applicability
bool myUtils::readCSV(const std::filesystem::path& filePath, Callable lineHandler, bool skipHeader) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cout << "Error: [" << filePath.string() << "] Could not open file." << std::endl;
        return false;
    }

    std::string line;

    if (skipHeader && std::getline(file, line)) {
        // First line read and discarded (header)
    }

    while (std::getline(file, line)) {
        // Optional: Handle potential Windows CRLF (\r\n) issues if the file might
        // have them and you're on a system that only expects \n.
        // std::getline usually handles this, but if \r remains:
        // if (!line.empty() && line.back() == '\r') {
        //     line.pop_back();
        // }
        lineHandler(line); // Call the user-provided function for each line
    }

    // Check for errors during read operations (other than EOF)
    if (file.bad()) {
        std::cout << "Error: [" << filePath.string() << "] I/O error while reading file." << std::endl;
        return false;
    }
    // file.eof() will be true if EOF was reached.
    // file.fail() might be true if getline failed not due to eof (e.g., badbit also set).
    // If badbit is not set and eof() is true, it's a successful read to the end.

    return true; // Successfully processed or reached EOF
}

vector<string> myUtils::split(const string &s, const char delimiter) {
    vector<string> result;

    stringstream ss(s);
    string temp;

    while(getline(ss,temp,delimiter)) {
        if(!temp.empty()) {
            result.push_back(temp);
        }
    }

    return result;
}

void myUtils::setEMCalDim(TH1* hist) {
    hist->GetXaxis()->SetLimits(0,256);
    hist->GetXaxis()->SetNdivisions(32, false);
    hist->GetXaxis()->SetLabelSize(0.04f);
    hist->GetXaxis()->SetTickSize(0.01f);
    hist->GetYaxis()->SetTickSize(0.01f);
    hist->GetYaxis()->SetLabelSize(0.04f);
    hist->GetYaxis()->SetLimits(0,96);
    hist->GetYaxis()->SetNdivisions(12, false);
    hist->GetYaxis()->SetTitleOffset(0.5);
    hist->GetXaxis()->SetTitleOffset(1);
}

pair<Int_t, Int_t> myUtils::getSectorIB(Int_t iphi, Int_t ieta) {
    Int_t k = iphi / m_ntowIBSide;

    Int_t sector = (ieta < 48) ? k + 32 : k;
    Int_t ib = (ieta < 48) ? m_nib_per_sector - ieta / m_ntowIBSide - 1: (ieta - 48) / m_ntowIBSide;

    return make_pair(sector, ib);
}

pair<Int_t, Int_t> myUtils::getSectorIB(Int_t towerIndex) {
    Int_t k = towerIndex / m_nchannel_per_sector;

    Int_t sector = (k % 2) ? (k - 1) / 2 : k / 2 + 32;
    Int_t ib = (k % m_nchannel_per_sector) / m_nchannel_per_ib;

    return make_pair(sector, ib);
}
