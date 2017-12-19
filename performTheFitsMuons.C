using namespace RooFit ;


TFile *myFile;

void doFit(TString type, TString nomCut, int ptBin, int etaBin, RooWorkspace *theWorkSpace, float *theEfficiency, float *theEffError){


    theWorkSpace->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll[0.9,0,1],numTot[1,0,1e10])");
    theWorkSpace->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    theWorkSpace->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg[0.9,0,1],fSigAll,numTot)");
    theWorkSpace->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    theWorkSpace->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    theWorkSpace->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    theWorkSpace->factory("expr::nPass('nSignalPass+nBkgPass',nSignalPass,nBkgPass)");
    theWorkSpace->factory("expr::nFail('nSignalFail+nBkgFail',nSignalFail,nBkgFail)");
    theWorkSpace->factory("SUM::tot(nPass*passing, nFail*failling)");
    theWorkSpace->factory("SUM::bgExtended(nBkgPass*backgroundPass,theZero[0]*backgroundFail)");

    RooCategory TnPcat("TnP","TnP") ;
    TnPcat.defineType("passing") ;
    TnPcat.defineType("failling") ;


    RooSimultaneous simPdf("simPdf","simultaneous pdf",TnPcat) ;
    simPdf.addPdf(*theWorkSpace->pdf("passing"),"passing") ;
    simPdf.addPdf(*theWorkSpace->pdf("failling"),"failling") ;


    TH1F *histoPass = (TH1F*) myFile->Get(nomCut+Form("_%i_%i_pass",ptBin,etaBin));
    TH1F *histoFail = (TH1F*) myFile->Get(nomCut+Form("_%i_%i_fail",ptBin,etaBin));
    TH1F *histoAll = (TH1F*) histoPass->Clone("histoAll");
    histoAll->Add(histoFail,1);


    RooDataHist *dataPass= new RooDataHist("dataPass","dataPass",*theWorkSpace->var("invMass"),histoPass);
    RooDataHist *dataFail = new RooDataHist("dataFail","dataFail",*theWorkSpace->var("invMass"),histoFail);
    RooDataHist *dataAll = new RooDataHist("dataAll","dataAll",*theWorkSpace->var("invMass"),histoAll);

    RooDataHist combData("combData","combined data",*theWorkSpace->var("invMass"),Index(TnPcat),Import("passing",*dataPass),Import("failling",*dataFail)) ;

    simPdf.fitTo(combData) ;
    simPdf.fitTo(combData) ;
    simPdf.fitTo(combData) ;

    //// cout << "failling = " << theWorkSpace->obj("nBkgPass")->getVal() << endl;

    RooPlot* xframe = theWorkSpace->var("invMass")->frame(Title("TnP model Passing"),Bins(50)) ;
    dataPass->plotOn(xframe);
    theWorkSpace->pdf("passing")->plotOn(xframe,LineColor(kGreen));
    theWorkSpace->pdf("passing")->plotOn(xframe,Components(*theWorkSpace->pdf("backgroundPass")), LineStyle(kDashed),LineColor(kGreen));

    RooPlot* xframe2 = theWorkSpace->var("invMass")->frame(Title("TnP model Failling"),Bins(50)) ;
    dataFail->plotOn(xframe2);
    theWorkSpace->pdf("failling")->plotOn(xframe2,LineColor(kRed));
    theWorkSpace->pdf("failling")->plotOn(xframe2,Components(*theWorkSpace->pdf("backgroundFail")), LineStyle(kDashed),LineColor(kRed));


    RooPlot* xframe3 = theWorkSpace->var("invMass")->frame(Title("TnP model Total"),Bins(50)) ;
    dataAll->plotOn(xframe3);
    theWorkSpace->pdf("tot")->plotOn(xframe3,LineColor(kBlue));


    int numTot = theWorkSpace->var("numTot")->getVal();
    float fSigAll = theWorkSpace->var("fSigAll")->getVal();
    float efficiency = theWorkSpace->var("efficiency")->getVal();
    float effBkg = theWorkSpace->var("effBkg")->getVal();

    *theEfficiency = theWorkSpace->var("efficiency")->getVal();
    *theEffError = theWorkSpace->var("efficiency")->getError();

    float efficiencyErr = theWorkSpace->var("efficiency")->getError();


    int nbPass = (int) (efficiency*fSigAll*numTot+effBkg*(1-fSigAll)*numTot);
    int nbFail = (int) ((1-efficiency)*fSigAll*numTot + (1-effBkg)*(1-fSigAll)*numTot);

    cout <<"nb pass " <<nbPass << endl;
    cout << "nbfail " << nbFail << endl;

    TCanvas *c0 = new TCanvas("c0","coucou",800,800);
	c0->SetFillColor(0);
    c0->Divide(2,2);

    c0->cd(1);
    xframe->Draw();


    c0->cd(2);
    xframe2->Draw();

    c0->cd(3);
    xframe3->Draw();

    c0->Print("fitPlots/"+type+"_"+nomCut+Form("_ptBin_%i_etaBin_%i.png",ptBin,etaBin));



}

void performTheFitsMuons(TString type){

    gStyle->SetOptStat(0);

    myFile = new TFile("histos_muons_"+type+".root");
    int nbPtBins = 3;
    float ptBins[4] = {20, 25, 50, 150};

    int nbEtaBins = 2;
    float etaBins[3] = {0, 1.479, 2.5};

    TH2F *efficiencies = new TH2F("efficiencies","",nbEtaBins, etaBins, nbPtBins, ptBins);


    RooWorkspace* voigtPlusExpo = new RooWorkspace("voigtPlusExpo");
    voigtPlusExpo->factory("invMass[90,70,120]");
    voigtPlusExpo->factory("Voigtian::theSig1p(invMass, mean1p[90,80,100], width1p[2.495], sigma1p[3,1,20])");
    voigtPlusExpo->factory("Voigtian::theSig2p(invMass, mean2p[90,80,100], width2p[2.495], sigma2p[3,1,20])");
    voigtPlusExpo->factory("SUM::signalPass(vPropp[0.8,0,1]*theSig1p,theSig2p)");
    voigtPlusExpo->factory("Voigtian::theSig1f(invMass, mean1f[90,80,100], width1f[3,1,2], sigma1f[3,0,20])");
    voigtPlusExpo->factory("Voigtian::theSig2f(invMass, mean2f[90,80,100], width2f[3,1,2], sigma2f[3,0,20])");
    voigtPlusExpo->factory("SUM::signalFail(vPropf[0.8,0,1]*theSig1f,theSig2f)");
    voigtPlusExpo->factory("Exponential::backgroundPass(invMass, lp[0,-5,5])");
    voigtPlusExpo->factory("Exponential::backgroundFail(invMass, lf[0,-5,5])");
    voigtPlusExpo->factory("efficiency[0.9,0,1]");
    voigtPlusExpo->factory("signalFractionInPassing[0.9]");


    float effVal, effErr;

    for (int i = 0 ; i < nbPtBins ; i++){
        for (int j = 0 ; j < nbEtaBins ; j++){
            doFit(type, "isTight",i,j,voigtPlusExpo, &effVal, &effErr);
            cout << "val=" << effVal << " error=" << effErr << endl;
            efficiencies->SetBinContent(j+1,i+1,effVal);
            efficiencies->SetBinError(j+1,i+1,effErr);

        }
    }
    TFile *myOutFile = new TFile("efficencyMuons"+type+".root","RECREATE");
    efficiencies->Write();
    myOutFile->Close();
}
