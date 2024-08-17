void    plot_emissionspectra()
{
    ifstream wlsfile, scfile;

    double wavelength(-1), intensity(-1), energy(-1);
    double sc_max_val = 711.85;
    double wls_max_val = 91.32;
    double maxval (0);
    char   dummy[300];

    TGraph *gScEnEmission = new TGraph();
    TGraph *gWLSEnEmission = new TGraph();
    TGraph *gScEmission = new TGraph();
    TGraph *gWLSEmission = new TGraph();

    scfile.open("./sc_emissionspectrum.dat");

    scfile.getline(dummy, 300);
    while(1)
    {
        scfile >> wavelength >> intensity;
        if(!scfile.good()) break;
        
        if(maxval < intensity) maxval = intensity;

        energy = 1240 / wavelength;
        printf("%6.3f %9.3f \n", energy, intensity/sc_max_val);
        gScEmission->SetPoint(gScEmission->GetN(), wavelength, intensity);
        gScEnEmission->SetPoint(gScEnEmission->GetN(), energy, intensity);
    }

    maxval = 0;
    scfile.close();

    wlsfile.open("./wls_emissionspectrum.dat");
    wlsfile.getline(dummy, 300);
    while(1)
    {
        wlsfile >> wavelength >> intensity;
        if(!wlsfile.good()) break;

        if(maxval < intensity) maxval = intensity;

        energy = 1240 / wavelength;
        //printf("%6.3f %9.3f \n", energy, intensity/wls_max_val);
        gWLSEnEmission->SetPoint(gWLSEnEmission->GetN(), energy, intensity);
        gWLSEmission->SetPoint(gWLSEmission->GetN(), wavelength, intensity);
    }
    wlsfile.close();

    cout << maxval << endl;

    gWLSEnEmission->SetMarkerStyle(kFullCircle);
    gWLSEnEmission->SetMarkerSize(0.6);
    gWLSEnEmission->GetYaxis()->SetRangeUser(0, 100);
    gScEnEmission->SetMarkerStyle(kFullCircle);
    gScEnEmission->SetMarkerSize(0.6);
    gScEnEmission->GetYaxis()->SetRangeUser(0, 800);

    gWLSEmission->SetMarkerStyle(kFullCircle);
    gWLSEmission->SetMarkerSize(0.6);
    gWLSEmission->GetYaxis()->SetRangeUser(0, 100);
    gScEmission->SetMarkerStyle(kFullCircle);
    gScEmission->SetMarkerSize(0.6);
    gScEmission->GetYaxis()->SetRangeUser(0, 800);

    TCanvas *c = new TCanvas();

    gScEnEmission->SetNameTitle("gScEnEmission","; Energy (eV); Intensity (x10^{6})");
    gWLSEnEmission->SetNameTitle("gWLSEnEmission", "; Energy (eV); Intensity (x10^6)");
    gScEmission->SetNameTitle("gScEmission","; Wavelength (nm); Intensity (x10^{6})");
    gWLSEmission->SetNameTitle("gWLSEmission", "; Wavelength (nm); Intensity (x10^6)");
    

    c->cd();
    //gScEmission->Draw("ACP");
    gScEnEmission->Draw("ACP");
    c->SaveAs("sc_emissionspectrum.eps");
    c->Clear();

    c->cd();
    gWLSEmission->Draw("ACP");
    c->SaveAs("wls_emissionspectrum.eps");
    c->Clear();
    delete c;


    TFile *ofile = new TFile("EmisionSpectra.root", "RECREATE");
    ofile->cd();
    
    gScEmission->Write();
    gWLSEmission->Write();

    ofile->Close();
}
