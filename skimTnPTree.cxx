#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"


void skimTnPTree() {
    TTree *tIn  = (TTree *) gFile->Get("AnalysisTree");
    Double_t tagEta, tagPt, probeEta;
    Bool_t probeIsTight;
    tIn->SetBranchAddress("tagEta", &tagEta);
    tIn->SetBranchAddress("tagPt", &tagPt);
    tIn->SetBranchAddress("probeEta", &probeEta);
    tIn->SetBranchAddress("probeIsTight", &probeIsTight);


    TFile *fOut = new TFile("/tmp/hbrun/tnp_treeMuonsPR.root", "RECREATE");
    fOut->mkdir("tpTree")->cd();
    TTree *tOut = tIn->CloneTree(0);
    Float_t AbsprobeEta;
    Int_t isTight;
    tOut->Branch("AbsprobeEta", &AbsprobeEta, "AbsprobeEta/F");
    tOut->Branch("isTight", &isTight, "isTight/I");
    int step = tIn->GetEntries()/1000;
    double evDenom = 100.0/double(tIn->GetEntries());
    TStopwatch timer; timer.Start();
    for (int i = 0, n = tIn->GetEntries(); i < n; ++i) {
        tIn->GetEntry(i);
        if (tagPt<25) continue;
        if (fabs(tagEta)>2.1) continue;
        AbsprobeEta = fabs(probeEta);
        isTight = probeIsTight;
        tOut->Fill();
        if ((i+1) % step == 0) {
            double totalTime = timer.RealTime()/60.; timer.Continue();
            double fraction = double(i+1)/double(n+1), remaining = totalTime*(1-fraction)/fraction;
            printf("Done %9d/%9d   %5.1f%%   (elapsed %5.1f min, remaining %5.1f min)\n", i, n, i*evDenom, totalTime, remaining);
            fflush(stdout);
        }
    }
    
    tOut->AutoSave(); // according to root tutorial this is the right thing to do
    fOut->Close();
    
    
}


