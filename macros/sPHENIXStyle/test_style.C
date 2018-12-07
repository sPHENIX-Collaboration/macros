//
{
  TCanvas c;
  TH1F h1("h1","H1;Mass [GeV];Counts [/ GeV];",20,0,20);
  TH1F h2("h2","H2;Mass [GeV];Counts [/ GeV];",20,0,20);
  for (int i = 0;i<1000;i++){h1.Fill(gRandom->Gaus(7,2));}
  for (int i = 0;i<1000;i++){h1.Fill(gRandom->Exp(.5)*20);}
  for (int i = 0;i<1000;i++){h2.Fill(gRandom->Exp(.5)*20);}
  h1.Draw("e1");
  h2.Draw("same");
  h2.SetFillColor(5);
  h1.Draw("e1same");

  TLegend leg(.55,.70,.85,.93);
  leg.AddEntry("","#it{#bf{sPHENIX}} Simulation","");
  leg.AddEntry("","Au+Au #sqrt{s_{NN}}=200 GeV","");
  leg.AddEntry(&h1,"Data","LP");
  leg.AddEntry(&h2,"MC","F");
  leg.Draw();
  c.RedrawAxis();
  c.Print("test_style.pdf");
  c.Print("test_style.png");

  TCanvas c2;
  c2.SetRightMargin(.2);
  TF2 f2("f2","exp(-x/5)*sin(y)",-5,5,-5,5);
  //f2.SetContour(100);
  f2.SetNpx(100);
  f2.SetNpy(100);
  f2.Draw("surf2z");
  f2.GetXaxis()->SetTitle("X");
  f2.GetYaxis()->SetTitle("Y");
  f2.GetZaxis()->SetTitle("Z");
  c2.Print("test_style_f2.pdf");
  c2.Print("test_style_f2.png");
}
