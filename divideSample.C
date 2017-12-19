#include <TRandom.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TFormula.h>
#include <TF1.h>
#include <TChain.h>
#include <TObject.h>
#include <TSystem.h>
#include <TClonesArray.h>
#include <TROOT.h>
#include <TLeaf.h>



//#include <iostream.h>
#include <string.h>
#include <sstream>

using namespace std;

//g++ MultiPhotonFilter_TrainingAndTest.C `root-config --libs --cflags` -o MultiPhotonFilter_TrainingAndTest



void WriteTreeInsideFile_FirstHalf(string SampleNameOutput, string SampleNameInput){

  TFile *input = new TFile(SampleNameInput.c_str());
  TTree *wholetree = (TTree*)input->Get("photons");
 
  wholetree->SetBranchStatus("*",1);
  
  int nInit = 0;
  int nEvent = (int)(wholetree->GetEntries()*0.5);
  TFile *newfile = new TFile(SampleNameOutput.c_str(),"recreate");
  newfile->cd();

  TTree *newtree = wholetree->CopyTree("", "", nEvent, nInit);
  newfile->cd();
  newtree->Write();
  newfile->Write();
  newfile->Close();
  
  delete wholetree;
  input->Close();

  return;
}

void WriteTreeInsideFile_SecondHalf(string SampleNameOutput, string SampleNameInput){

  TFile *input = new TFile(SampleNameInput.c_str());
  TTree *wholetree = (TTree*)input->Get("photons");
 
  wholetree->SetBranchStatus("*",1);
  
  //nInit = 0;
  //nEvent = (int)(wholetree->GetEntries()*0.5);
  int nInit = (int)(wholetree->GetEntries()*0.5)+1;
  int nEvent = wholetree->GetEntries();

  TFile *newfile = new TFile(SampleNameOutput.c_str(),"recreate");
  newfile->cd();

  TTree *newtree = wholetree->CopyTree("", "", nEvent, nInit);
  newfile->cd();
  newtree->Write();
  newfile->Write();
  newfile->Close();
  
  delete wholetree;
  input->Close();

  return;
}

void WriteTreeInsideFile_AnyThird(string SampleNameOutput, string SampleNameInput, int numpart){



  TFile *input = new TFile(SampleNameInput.c_str());
  TTree *wholetree = (TTree*)input->Get("photons");
 
  wholetree->SetBranchStatus("*",1);
  
  //nInit = 0;
  //nEvent = (int)(wholetree->GetEntries()*0.5);
  int nInit; //= (int)(wholetree->GetEntries()*0.5)+1;
  int nEvent; //= wholetree->GetEntries();

  if (numpart==0){
    nInit = 0;
    nEvent = (int)((wholetree->GetEntries())/3.0);
  }
  if (numpart==1){
    nInit = (int)((wholetree->GetEntries())/3.0)+1;
    nEvent = (int)(2.0*(wholetree->GetEntries())/3.0);
    
  }
  if (numpart==2){
    nInit = (int)(2.0*(wholetree->GetEntries())/3.0)+1;
    nEvent = wholetree->GetEntries();
  }

  stringstream ss[2];
  string snInit, snEvent;
  ss[0] << nInit;
  ss[0] >> snInit;
  ss[1] << nEvent;
  ss[1] >> snEvent;

  selection = "Entry$ >= " + snInit + " && Entry$ <= " + snEvent;

  cout << "nInit="<<nInit<<" nEvent="<<nEvent<<endl;

  TFile *newfile = new TFile(SampleNameOutput.c_str(),"recreate");
  newfile->cd();

  TTree *newtree = wholetree->CopyTree(selection.c_str(), "", nEvent, nInit);
  newfile->cd();
  //newtree->Write();
  newfile->Write("Tree");
  newfile->Close();
  
  delete wholetree;
  input->Close();

  return;
}

int MultiPhotonFilter_DivideSampleInTwo(){

  cout << "A"<<endl;

  string Sample[2];
  Sample[0] = "MC_QCD_Pt20";
  Sample[1] = "MC_PhotonJet_Pt15toInf";
  
  cout << "B"<<endl;

  //string Dir = "/sps/cms/chanon/CMSSW_3_5_6/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/InputHiggsSearches_Kfactors/";
  string Dir = "/sps/cms/chanon/CMSSW_3_6_2/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/Input/";
  

  //string prefixe = "DiphotonTree_";
  //string prefixe = "TMVAoutput_";
  string prefixe = "GammaPi0discTemplatesVar_";
//  string suffixe = "_GOODCOLLfilter.root";
  //string suffixe =  "_GoodCollEGfilter.root";
  string suffixe = ".root";
  //string suffixe = "";

  //string DirOutput = "/sps/cms/chanon/CMSSW_3_5_8_patch3/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/SinglePhotonPurityAnalysis_afterTraining_pt15_V26B/TemplateMock/";
  //string DirOutput = "/sps/cms/chanon/CMSSW_3_5_8_patch3/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/SinglePhotonPurityAnalysis_beforeTraining/";
  //string DirOutput = "/sps/cms/chanon/CMSSW_3_5_8_patch3/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/DiphotonFiltered_pt20mgg40_withNN_pt15_V26B_Isolated/";
  //string DirOutput = "/sps/cms/chanon/CMSSW_3_5_6/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/InputHiggsSearches_new/";
  //string DirOutput = "/sps/cms/chanon/CMSSW_3_5_6/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/InputHiggsSearches_Kfactors/";
  string DirOutput =  "/sps/cms/chanon/CMSSW_3_6_2/src/UserCode/IpnTreeProducer/test/GammaPi0discTemplatesVar/MiniTrees/Input/";

  //string suffixeOutput = "_GOODCOLLfilter";
  //string suffixeOutput = "_GoodCollEGfilter";
  string suffixeOutput = "";

  string SkimType="";
  //string SkimType="HiggsSearches";
  //string SkimType = "ClusterShape";
  //string SkimType = "Env";
  //string SkimType="ClusterShapeFake";
  //string SkimType="EnvFake";

  string FileName[14];
  string FileNameOutput[14];
  string FileNameOutput_Training[14];
  string FileNameOutput_Test[14];  
  string FileNameOutput_TestTemplate[14];  
  
  cout << "E"<<endl;

  for (int i=0; i<2; i++){
    FileName[i] = Dir  + prefixe + Sample[i] + suffixe;
    FileNameOutput[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + ".root";
    FileNameOutput_Training[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + "_Training.root";
    FileNameOutput_Test[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + "_Test.root";
    //FileNameOutput_Training[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + "_Mock.root";
    //FileNameOutput_Test[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + "_Template.root";
    //FileNameOutput_Training[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + "_TrainingOneThird.root";
    //FileNameOutput_Test[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + "_TestMockTwoThird.root";
    //FileNameOutput_TestTemplate[i] = DirOutput + prefixe + Sample[i] + suffixeOutput + SkimType + "_TestTemplateThreeThird.root";
  }



  int nEvent, nInit;

  //gSystem->Load("$ROOTSYS/test/libEvent.so");

  cout << "F"<<endl;

  TFile *input;
  TTree *wholetree;

  cout << "C"<<endl;

  for (int is=0; is<2; is++){
    
    cout << "Dividing "<<FileName[is].c_str()<<endl;

    //if (is!=0 && is!=3) continue;

    string SampleName = FileName[is];

    //if (is!=20 && is!=23) continue;

    string SampleNameTraining = FileNameOutput_Training[is];
    string SampleNameTest = FileNameOutput_Test[is];
    string SampleNameTemplate = FileNameOutput_TestTemplate[is];

    WriteTreeInsideFile_FirstHalf(SampleNameTraining, SampleName);
    WriteTreeInsideFile_SecondHalf(SampleNameTest, SampleName);

    //WriteTreeInsideFile_AnyThird(SampleNameTraining, SampleName, 0);
    //WriteTreeInsideFile_AnyThird(SampleNameTest, SampleName, 1); 
    //WriteTreeInsideFile_AnyThird(SampleNameTemplate, SampleName, 2);   

    //input = new TFile(SampleName.c_str());
    //wholetree = (TTree*)input->Get("Tree");

    //wholetree->SetBranchStatus("*",1);
    
    //nInit = 0;
    //nEvent = (int)(wholetree->GetEntries()*0.5);
    //WriteTreeInsideFile(SampleNameTraining, SampleName,  nInit,  nEvent);

    /*
    TFile *newfile_training = new TFile(SampleNameTraining.c_str(),"recreate");
    TTree *newtree_training = wholetree->CopyTree("", "", nEvent, nInit);
    newfile_training->Write();
    newfile_training->Close();
    */

    /*
    nInit = (int)(wholetree->GetEntries()*0.5)+1;
    nEvent = wholetree->GetEntries();
    TFile *newfile_test = new TFile(SampleNameTest.c_str(),"recreate");
    TTree *newtree_test = wholetree->CopyTree("", "", nEvent, nInit);   
    newfile_test->Write();
    newfile_test->Close();
    //newtree->GetBranch("fH")->SetFile("small_fH.root");
    //newtree->CopyEntries(oldtree);
    

    */
    //newtree->Print();
    //delete wholetree;
    //input->Close();

  }

  return;

  for (int is=0; is<6; is++){

    string SampleName = FileName[is];

    //if (is!=20 && is!=23) continue;

    string SampleNameTraining = FileNameOutput_Training[is];
    string SampleNameTest = FileNameOutput_Test[is];

    TFile *input = new TFile(SampleName.c_str());
    TTree *wholetree = (TTree*)input->Get("Tree");

    wholetree->SetBranchStatus("*",1);
  
    //nInit = 0;
    nInit = (int)(wholetree->GetEntries()*0.5)+1;
    nEvent = wholetree->GetEntries();
    TFile *newfile_test = new TFile(SampleNameTest.c_str(),"recreate");
    TTree *newtree_test = wholetree->CopyTree("", "", nEvent, nInit);   
    newfile_test->Write();
    newfile_test->Close();
    //newtree->GetBranch("fH")->SetFile("small_fH.root");
    //newtree->CopyEntries(oldtree);
    
    input->Close();

    //newtree->Print();


  }


  return;




  Int_t isFromQuarkGen, isPromptGenPho, isGenElectron, classification;
  Float_t Et, EcalIso, HcalIso;
  Float_t ptoverjetpt, DrSCclosest, DrTrkClosest;

  for (int is=0; is<27; is++){

    if (is!=20 && is!=23) continue;

     cout << "--- Opening input sample" << endl;

     string SampleName = FileName[is];
     string SampleNameNew = FileNameOutput[is];

     cout << "--- Input  : "<<SampleName.c_str()<<endl;
     cout << "--- Output : "<<SampleNameNew.c_str()<<endl;

     TFile *inputS = new TFile(SampleName.c_str());
     TTree *signal     = (TTree*)inputS->Get("Tree");
     
     TLeaf* Leaf_isFromQuarkGen = signal->GetLeaf("pho_isFromQuarkGen");
     TLeaf* Leaf_isPromptGenPho = signal->GetLeaf("pho_isPromptGenPho");
     TLeaf* Leaf_isGenElectron = signal->GetLeaf("pho_isGenElectron");
     TLeaf* Leaf_Et = signal->GetLeaf("pho_et");
     TLeaf* Leaf_EcalIso = signal->GetLeaf("pho_IsoEcalRechit");
     TLeaf* Leaf_HcalIso = signal->GetLeaf("pho_IsoHcalRechit");
     TLeaf* Leaf_ptoverjetpt = signal->GetLeaf("pho_ptoverjetpt_pt15");
     TLeaf* Leaf_DrSCclosest = signal->GetLeaf("pho_DrSCclosest");
     TLeaf* Leaf_DrTrkClosest = signal->GetLeaf("pho_DrTrkclosest_pt2");


     int nLeafInt=0;
     TObjArray* listofleaves = signal->GetListOfLeaves();
     int nLeaf = signal->GetNbranches();
     string* LeafNames = new string[nLeaf];
     string* LeafTypes = new string[nLeaf];
     for (int i=0; i<nLeaf; i++){
       LeafNames[i] = listofleaves->At(i)->GetName();
       LeafTypes[i] = signal->GetLeaf(LeafNames[i].c_str())->GetTypeName();
       cout << "Var "<<i<<" : "<<LeafTypes[i].c_str() <<", "<<LeafNames[i].c_str()<<endl;

       if (LeafTypes[i]=="Int_t") nLeafInt++;
     }

     cout << "nLeafInt="<<nLeafInt<<endl;
   
     Float_t* Variable = new Float_t[nLeaf-nLeafInt];
     Int_t* VariableInt = new Int_t[nLeafInt];

     int iLeafInt=0;
     int iLeafFloat=0;
     int iVarSelectedSkim[6];
     for (int i=0; i<nLeaf; i++){

       if (LeafTypes[i]=="Int_t") {
	 signal->SetBranchAddress(LeafNames[i].c_str(), &VariableInt[iLeafInt]);
	 iLeafInt++;
       }
       else {
	 signal->SetBranchAddress(LeafNames[i].c_str(), &Variable[iLeafFloat]);
	 iLeafFloat++;
       }

     }


     TFile *outputS = new TFile(SampleNameNew.c_str(),"recreate");
     TTree *OutputTree = new TTree("Tree","Photons info");

     string LeafNameBis;
     iLeafInt=0;
     iLeafFloat=0;
     for (int i=0; i<nLeaf; i++){
       if (LeafTypes[i]=="Int_t") {
	 LeafNameBis = LeafNames[i] + "/I";
	 OutputTree->Branch(LeafNames[i].c_str(), &VariableInt[iLeafInt], LeafNameBis.c_str());
	 iLeafInt++;
       }
       else {
	 LeafNameBis = LeafNames[i] + "/F";
	 OutputTree->Branch(LeafNames[i].c_str(), &Variable[iLeafFloat], LeafNameBis.c_str());
	 iLeafFloat++;
       }

     }
     
     for (Long64_t ievt=0; ievt<signal->GetEntries();ievt++) {
       
       if (ievt%10000 == 0)      cout << "--- ... Processing event: " << ievt << endl;

       signal->GetEntry(ievt);
       
       isFromQuarkGen = Leaf_isFromQuarkGen->GetValue();
       isPromptGenPho = Leaf_isPromptGenPho->GetValue();
       isGenElectron = Leaf_isGenElectron->GetValue();
       Et = Leaf_Et->GetValue();
       EcalIso = Leaf_EcalIso->GetValue();
       HcalIso = Leaf_HcalIso->GetValue();
       ptoverjetpt = Leaf_ptoverjetpt->GetValue();
       DrSCclosest = Leaf_DrSCclosest->GetValue();
       DrTrkClosest = Leaf_DrTrkClosest->GetValue();
//       classification = Leaf_classification->GetValue();      
 
       if (SamplesType[is]=="Bkgd" && (isFromQuarkGen!=0 || Et<20 || EcalIso>4 || HcalIso>2)) continue;
       if ((SamplesType[is]=="BkgdPhJet") && (isPromptGenPho!=0 && isFromQuarkGen!=0 || Et<20 || EcalIso>4 || HcalIso>2)) continue;
       if (SamplesType[is]=="Signal" && (isPromptGenPho!=1 ||  Et<20 || EcalIso>4 || HcalIso>2)) continue;
//       if (SamplesType[is]=="SignalZee" && (isGenElectron!=1 ||  Et<20 || EcalIso>4 || HcalIso>2 || classification!=0)) continue;
       
       if ((SkimType=="Env" || SkimType=="EnvFake") && (ptoverjetpt==-1 || DrSCclosest==10 || DrTrkClosest==10)) continue;

       OutputTree->Fill();

     }


     cout << "--- End of event loop: "<<endl;
  
 
     outputS->cd();
     outputS->Write();
     outputS->Close();


  }

  return 0;
}

