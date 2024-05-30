void plot_nodes()
{
	TFile *file = new TFile("HIST_CALOR_QA-00043901-0004.root", "READ");
	
	TString name = "h_CaloValid_nodes_exists";
	TH1F *hist = (TH1F*)file->Get(name.Data());

	TCanvas *c1 = new TCanvas("c1", "Histogram with Bin Labels", 800, 600);

	hist->Draw();

	//label each bin
	
	std::string binNames[] = {"GlobalVertexMap", "TOWERINFO_CALIB_CEMC", "TOWERINFO_CALIB_HCALIN", "TOWERINFO_CALIB_HCALOUT", "TOWERINFO_CALIB_ZDC", "TOWERS_ZDC", "TOWERS_CEMC", "TOWERS_HCALOUT", "TOWERS_HCALIN", "MbdPmtContainer", "CLUSTERINFO_POS_COR_CEMC"};

	int size = sizeof(binNames)/sizeof(binNames[0]);
	
	for (int i=0; i<size; i++)
	{
		std::cout << binNames[i] << std::endl;
	}

	for (int i = 1; i <= hist->GetNbinsX(); i++)
	{
		float binCenter = hist->GetBinCenter(i);
		float binContent = hist->GetBinContent(i);
		// Create a TText object
		TText *text = new TText(binCenter, binContent + 1, binNames[i-1].c_str());
		//TText *text = new TText(binCenter, binContent + 1, Form(binNames[i-1].c_str()));
		//TText *text = new TText(binCenter, binContent + 1, Form("Bin %d", i));
		// Set text alignment (centered)
		text->SetTextAlign(22);
		text->SetTextSize(0.04);
		text->SetTextAngle(90);
		text->Draw();
	}

	c1->Update();
}
