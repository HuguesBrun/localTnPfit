TChain *chain = new TChain("tpTree/AnalysisTree");
TFile *myFile = new TFile("histos_muons_data.root","RECREATE");

TString baseAcceptCut = "tagPt>25&&abs(tagEta)<2.1&&invMass>60&&invMass<120";
TString theCutToTest = "probeIsTight==1";
TString shortNameCut = "isTight";

int nbPtBins = 3;
float ptBins[4] = {20, 25, 50, 150};

int nbEtaBins = 2;
float etaBins[3] = {0, 1.479, 2.5};


doTheHistos(TString nameHisto, TString testCut, TString baseCut){
    cout << "fill the histo:" << nameHisto << endl;
    
    
    TH1F *passing = new TH1F("passing","",40,60,120);
    chain->Draw("invMass>>passing",baseCut+"&&"+testCut);
    myFile->cd();
    passing->Write(nameHisto+"_pass");
    delete passing;
    
    TH1F *failling = new TH1F("failling","",40,60,120);
    chain->Draw("invMass>>failling",baseCut+"&&(!("+testCut+"))");
    myFile->cd();
    failling->Write(nameHisto+"_fail");
    delete failling;
    
}


makeHistos(TString nameFile){
    chain->Add(nameFile);
    
    
    for (int i = 0 ; i < nbPtBins; i++){
        for (int j = 0 ; j < nbEtaBins ; j++){
            TString theCut = Form("probePt>%f&probePt<%f&&abs(probeEta)>%f&&abs(probeEta)<%f",ptBins[i], ptBins[i+1], etaBins[j], etaBins[j+1]);
            doTheHistos(shortNameCut+Form("_%i_%i",i,j),theCutToTest,baseAcceptCut+"&&"+theCut);

        }
    }
    
    
    
    
}