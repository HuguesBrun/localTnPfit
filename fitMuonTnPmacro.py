import FWCore.ParameterSet.Config as cms
import re, os


import sys
args = sys.argv[1:]
if (sys.argv[0] == "cmsRun"): args =sys.argv[2:]
scenario = "data_all"
if len(args) > 0: scenario = args[0]
print "Will run scenario ", scenario 

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

Template = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),

    Variables = cms.PSet(
        invMass = cms.vstring("Tag-muon Mass", "60", "120", "GeV/c^{2}"),
        probePt = cms.vstring("muon p_{T}", "0", "1000", "GeV/c"),
        probEta    = cms.vstring("muon #eta", "-2.5", "2.5", ""),
        AbsprobeEta = cms.vstring("muon |#eta|", "0", "2.5", ""),

    ),

    Categories = cms.PSet(
                          isTight = cms.vstring("isTight","dummy[pass=1,fail=0]"),
      
    ),
    PDFs = cms.PSet(

        voigtPlusExpo = cms.vstring(
                                    "Voigtian::theSig1p(invMass, mean1p[90,80,100], width1p[2.495], sigma1p[3,1,20])",
                                    "Voigtian::theSig2p(invMass, mean2p[90,80,100], width2p[2.495], sigma2p[3,1,20])",
                                    "SUM::signalPass(vPropp[0.8,0,1]*theSig1p,theSig2p)",
                                    "Voigtian::theSig1f(invMass, mean1f[90,80,100], width1f[3,1,2], sigma1f[3,0,20])",
                                    "Voigtian::theSig2f(invMass, mean2f[90,80,100], width2f[3,1,2], sigma2f[3,0,20])",
                                    "SUM::signalFail(vPropf[0.8,0,1]*theSig1f,theSig2f)",
                                    "Exponential::backgroundPass(invMass, lp[0,-5,5])",
                                    "Exponential::backgroundFail(invMass, lf[0,-5,5])",
                                    "efficiency[0.9,0,1]",
                                    "signalFractionInPassing[0.9]"
        ),
                    voigtPlusExpoFromMChighEtaBin = cms.vstring(
                                                                "Voigtian::theSig1p(invMass, mean1p[90,80,100], width1p[2.495], sigma1p[3,1,20])",
                                                                "Voigtian::theSig2p(invMass, mean2p[90,80,100], width2p[2.495], sigma2p[3,1,20])",
                                                                "SUM::sVoigP(vPropp[0.8,0,1]*theSig1p,theSig2p)",    
                                                                "Voigtian::theSig1f(invMass, mean1f[90,80,100], width1f[3,1,2], sigma1f[3,0,20])",
                                                                "Voigtian::theSig2f(invMass, mean2f[90,80,100], width2f[3,1,2], sigma2f[3,0,20])",
                                                                "SUM::sVoigF(vPropf[0.8,0,1]*theSig1f,theSig2f)",    
                                                                "Exponential::expP(invMass, lsp[0,-5,5])",
                                                                "Exponential::expF(invMass, lsf[0,-5,5])",
                                                                "ArgusBG::argF(invMass, ap0[90,88,93], ap1[-5,-5,-0.5])",
                                                                "SUM::shapeF(vFrac[0.8,0.5,1]*argF, expF)",
                                                                "SUM::signalPass(vPropTotp[0.8,0.5,1]*sVoigP,expP)",
                                                                "SUM::signalFail(vPropTotf[0.8,0.5,1]*sVoigF,shapeF)",
                                                                "Exponential::backgroundPass(invMass, lp[0])",
                                                                "Exponential::backgroundFail(invMass, lf[0])",
                                                                "efficiency[0.9,0,1]",
                                                                "signalFractionInPassing[1]"
                                                                ),
                
                    
                    voigtPlusExpoFromMChighPtBin = cms.vstring(
                                                               "CBShape::crystal(invMass, mean[90,80,100], sigma[3,1,20],alpha[3., 0.5, 5.], n[1, 0., 100.])",
                                                               "RooLandau::pLandau(invMass, Lmp[95,90,100],wp[1,0,10])",
                                                               "ArgusBG::fArg(invMass, ap0[90,88,93], ap1[-5,-6,-0.5])",
                                                               "RooLandau::fLandau(invMass, Lmf[95,90,100],wf[1,0,10])",
                                                               "SUM::argLand(vFrac[0.8,0.5,1]*fLandau, fArg)",
                                                               "SUM::signalPass(vProp[0.8,0.5,1]*crystal,pLandau)",
                                                               "SUM::signalFail(vProp[0.8,0.5,1]*crystal,argLand)",
                                                               "Exponential::backgroundPass(invMass, lp[0])",
                                                               "Exponential::backgroundFail(invMass, lf[0])",
                                                               "efficiency[0.9,0,1]",
                                                               "signalFractionInPassing[1]"
                                                      ),
    ),

    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(40),
    Efficiencies = cms.PSet(
    ), # will be filled later
)



PT_ETA_BINS = cms.PSet(
                       probePt   = cms.vdouble(20, 25, 50, 150),
                       AbsprobeEta = cms.vdouble(0, 1.479, 2.5)
)




PREFIX="file:/afs/cern.ch/work/h/hbrun/treesForPR/"
process.TnP_MuonID = Template.clone(
    InputFileNames = cms.vstring(PREFIX+"tnpZ_withMVAIsoNew.root"),
    InputTreeName = cms.string("AnalysisTree"),
    InputDirectoryName = cms.string("tpTree"),
    OutputFileName = cms.string("TnP_MuonID_%s.root" % scenario),
    Efficiencies = cms.PSet(),
)



if "data" in scenario:
    if   "muons" in scenario: process.TnP_MuonID.InputFileNames = [ PREFIX+"tnp_treeMuonsPR.root" ]
    else: process.TnP_MuonID.InputFileNames = [ PREFIX+""]

if "mc" in scenario:
    if "muons" in scenario: process.TnP_MuonID.InputFileNames = [PREFIX + ""]
    else: process.TnP_MuonID.InputFileNames = [ PREFIX+""]

IDS = ["isTight"]


ALLBINS = [("PromptRate",PT_ETA_BINS)]



if len(args) > 1 and args[1] not in IDS: IDS += [ args[1] ]
for ID in IDS:
    if len(args) > 1 and ID != args[1]: continue
    
    for X,B in ALLBINS:
        if len(args) > 2 and X not in args[2:]: continue
        module = process.TnP_MuonID.clone(OutputFileName = cms.string("TnP_MuonID_%s_%s_%s.root" % (scenario, ID, X)))
        shape = cms.vstring("voigtPlusExpo")

        DEN = B.clone(); num = ID;
        if "_from_" in ID:
            parts = ID.split("_from_")
            num = parts[0]
            setattr(DEN, parts[1], cms.vstring("pass"))
        print shape

        setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(num,"pass"),
            UnbinnedVariables = cms.vstring("invMass"),
            BinnedVariables = DEN,
            BinToPDFmap = shape #cms.vstring(shape)
        ))

        setattr(process, "TnP_MuonID_"+ID+"_"+X, module)        
        setattr(process, "run_"+ID+"_"+X, cms.Path(module))

