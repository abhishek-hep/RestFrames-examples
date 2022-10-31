/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_ttW.C
///
///  \author Abhishek Sharma
///          (abhishek@cern.ch)
///
///  \date   2022 October
//
/////////////////////////////////////////////////////////////////////////

#define COMPILER (!defined(__CINT__) && !defined(__CLING__))
#if defined(__MAKECINT__) || defined(__ROOTCLING__) || COMPILER
#include "RestFrames/RestFrames.hh"
#include <chrono>
#else
RestFrames::RFKey ensure_autoload(1);
#endif

using namespace RestFrames;

void example_ttW(const std::string output_name = "output_ttW.root") {

    double mT = 173.21;
    double mW = 80.385;
    double mB = 4.18;
    double mL = 0.106;
    double mN = 0.;
    double wW = 2.085;
    double wT = 1.9;
    // number of events to generate
    int Ngen = 1000000;

    g_Log << LogInfo << "Initializing generator" << LogEnd;

    ppLabGenFrame LAB_Gen("LAB_Gen","LAB");
    DecayGenFrame CM_Gen("CM_Gen","CM");
    DecayGenFrame TT_Gen("TT_Gen","t #bar{t}");
    ResonanceGenFrame W_Gen("W_Gen","W");
    VisibleGenFrame l_Gen("l_Gen","#it{l}");
    InvisibleGenFrame n_Gen("n_Gen","#nu");
    ResonanceGenFrame Ta_Gen("Ta_Gen","t_{a}");
    ResonanceGenFrame Tb_Gen("Tb_Gen","T_{b}");
    ResonanceGenFrame Wa_Gen("Wa_Gen","W_{a}");
    ResonanceGenFrame Wb_Gen("Wb_Gen","W_{b}");
    VisibleGenFrame ba_Gen("ba_Gen","b_{a}");
    VisibleGenFrame bb_Gen("bb_Gen","b_{b}");
    VisibleGenFrame j1a_Gen("j1a_Gen","j_{1,a}");
    VisibleGenFrame j2a_Gen("j2a_Gen","j_{1,a}");
    VisibleGenFrame lb_Gen("lb_Gen","l_{b}");
    InvisibleGenFrame nb_Gen("nb_Gen","#nu_{b}");

    LAB_Gen.SetChildFrame(CM_Gen);
    CM_Gen.AddChildFrame(Ta_Gen);
    CM_Gen.AddChildFrame(Tb_Gen);
    CM_Gen.AddChildFrame(W_Gen);
    W_Gen.AddChildFrame(l_Gen);
    W_Gen.AddChildFrame(n_Gen);
    Ta_Gen.AddChildFrame(Wa_Gen);
    Ta_Gen.AddChildFrame(ba_Gen);
    Tb_Gen.AddChildFrame(Wb_Gen);
    Tb_Gen.AddChildFrame(bb_Gen);
    Wa_Gen.AddChildFrame(j1a_Gen);
    Wa_Gen.AddChildFrame(j2a_Gen);
    Wb_Gen.AddChildFrame(lb_Gen);
    Wb_Gen.AddChildFrame(nb_Gen);

    if(LAB_Gen.InitializeTree())
        g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
    else
        g_Log << LogError << "...Failed initializing generator tree" << LogEnd;

    CM_Gen.SetVariableMass();
    Ta_Gen.SetMass(mT);     Tb_Gen.SetMass(mT);
    Ta_Gen.SetWidth(wT);     Tb_Gen.SetWidth(wT);
    Wa_Gen.SetMass(mW);     Wb_Gen.SetMass(mW);     W_Gen.SetMass(mW);
    Wa_Gen.SetWidth(wW);    Wb_Gen.SetWidth(wW);    W_Gen.SetWidth(wW);
    ba_Gen.SetMass(mB);     bb_Gen.SetMass(mB);
    l_Gen.SetMass(mL);      lb_Gen.SetMass(mL);
    n_Gen.SetMass(mN);      nb_Gen.SetMass(mN);

    ba_Gen.SetPtCut(20.);          bb_Gen.SetPtCut(20.);
    ba_Gen.SetEtaCut(2.5);         bb_Gen.SetEtaCut(2.5);
    l_Gen.SetPtCut(15.);          lb_Gen.SetPtCut(15.);
    l_Gen.SetEtaCut(2.5);         lb_Gen.SetEtaCut(2.5);

    if(LAB_Gen.InitializeAnalysis())
        g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
    else
        g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;

    g_Log << LogInfo << "Initializing reconstruction frames and tree..." << LogEnd;

    LabRecoFrame LAB("LAB","LAB");
    DecayRecoFrame CM("CM","CM");
    DecayRecoFrame TT("TT","t #bar{t}");
    DecayRecoFrame W("W","W");
    VisibleRecoFrame l("l","#it{l}");
    InvisibleRecoFrame n("n","#nu");
    DecayRecoFrame Ta("Ta","t_{a}");
    DecayRecoFrame Tb("Tb","t_{b}");
    VisibleRecoFrame Wa("Wa","W_{a}");
    DecayRecoFrame Wb("Wb","W_{b}");
    VisibleRecoFrame ba("ba","b_{a}");
    VisibleRecoFrame bb("bb","b_{b}");
    VisibleRecoFrame lb("lb","#it{l}_{b}");
    InvisibleRecoFrame nb("nb","#nu_{b}");

    LAB.SetChildFrame(CM);
    CM.AddChildFrame(W);
    CM.AddChildFrame(Ta);
    CM.AddChildFrame(Tb);
    W.AddChildFrame(l);
    W.AddChildFrame(n);
    Ta.AddChildFrame(ba);
    Tb.AddChildFrame(bb);
    Ta.AddChildFrame(Wa);
    Tb.AddChildFrame(Wb);
    Wb.AddChildFrame(lb);
    Wb.AddChildFrame(nb);

    if(LAB.InitializeTree())
        g_Log << LogInfo << "...Successfully initialized reconstruction trees" << LogEnd;
    else
        g_Log << LogError << "...Failed initializing reconstruction trees" << LogEnd;

    //Invisible group
    InvisibleGroup INV("INV","neutrino jigsaws for same W mass");
    INV.AddFrame(n);
    INV.AddFrame(nb);

    //Combinatorics for leptons and b jets
    CombinatoricGroup COMB_L("COMB_L","lepton jigsaws");
    COMB_L.AddFrame(l);
    COMB_L.AddFrame(lb);
    COMB_L.SetNElementsForFrame(l,1);
    COMB_L.SetNElementsForFrame(lb,1);

    CombinatoricGroup COMB_B("COMB_B","jet assignment jigsaws");
    COMB_B.AddFrame(ba);
    COMB_B.AddFrame(bb);
    COMB_B.AddFrame(Wa);
    COMB_B.SetNElementsForFrame(ba,1);
    COMB_B.SetNElementsForFrame(bb,1);
    COMB_B.SetNElementsForFrame(Wa,2);

    // Minimise equal W masses 
    SetMassInvJigsaw NuNuM("NuNuM","neutrino invariant mass");
    INV.AddJigsaw(NuNuM);

    SetRapidityInvJigsaw NuNuR("NuNuR","neutrino rapidity");
    INV.AddJigsaw(NuNuR);
    NuNuR.AddVisibleFrames(l+lb);

    MaxProbBreitWignerInvJigsaw Invisible("Invisible","MET splitting",2);
    INV.AddJigsaw(Invisible);
    Invisible.AddVisibleFrame(lb,0);
    Invisible.AddInvisibleFrame(nb,0);
    Invisible.AddVisibleFrame(l,1);
    Invisible.AddInvisibleFrame(n,1);
    Invisible.SetPoleMass(80.379,0);
    Invisible.SetPoleMass(80.379,1);
    Invisible.SetWidth(2.085,0);
    Invisible.SetWidth(2.085,1);

    // Combinatorics
    MinMassesCombJigsaw Leps("Leps","Lepton combinatoric jigsaw");
    COMB_L.AddJigsaw(Leps);
    Leps.AddCombFrame(Wb,0);
    Leps.AddCombFrame(W,1);
    Leps.AddObjectFrame(Wb,0);
    Leps.AddObjectFrame(W,1);

    MinMassChi2CombJigsaw JETSa("JETSa","jets for top a",2,2);
    COMB_B.AddJigsaw(JETSa);
    JETSa.AddCombFrame(ba,0);
    JETSa.AddCombFrame(Wa,1);
    JETSa.AddObjectFrame(Ta,0);
    JETSa.AddObjectFrame(Wa,1);
    JETSa.SetMass(173.,0);
    JETSa.SetMass(80.379,1);
    JETSa.SetSigma(3.,0);
    JETSa.SetSigma(3.,1);

    MinMassChi2CombJigsaw JETS("JETS","jets between tops",2,2);
    COMB_B.AddJigsaw(JETS);
    JETS.AddCombFrames(ba+Wa,0);
    JETS.AddCombFrame(bb,1);
    JETS.AddObjectFrame(Ta,0);
    JETS.AddObjectFrame(Tb,1);
    JETS.SetMass(173.,0);
    JETS.SetMass(173.,1);
    JETS.SetSigma(3.,0);
    JETS.SetSigma(3.,1);

    if(LAB.InitializeAnalysis())
        g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
    else
        g_Log << LogError << "...Failed initializing analysis" << LogEnd;

    TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");

    treePlot->SetTree(LAB_Gen);
    treePlot->Draw("GenTree", "Generator Tree", true);

    treePlot->SetTree(LAB);
    treePlot->Draw("RecoTree", "Reconstruction Tree");

    treePlot->SetTree(COMB_B);
    treePlot->Draw("COMB_B", "b-jet Jigsaws", true);

    treePlot->SetTree(COMB_L);
    treePlot->Draw("COMB_L", "lepton Jigsaws", true);

    treePlot->SetTree(INV);
    treePlot->Draw("InvTree", "Inivisible Jigsaws", true);


    std::string plot_title = "t #bar{t} W #rightarrow bb W(#it{l} #nu) W(jj) W(#it{l} #nu)";
    HistPlot* histPlot = new HistPlot("HistPlot", plot_title);

    const HistPlotCategory& cat_Reco  = histPlot->GetNewCategory("Reco", "Reconstruction");

    const HistPlotVar& MCM    = histPlot->GetNewVar("M_CM", "M_{CM} / m_{CM}^{ true}", 0., 2.);
    const HistPlotVar& MW   = histPlot->GetNewVar("MW", "M_{W} [GeV]", 0., 120.);
    const HistPlotVar& MWa   = histPlot->GetNewVar("MWa", "M_{W}^{a} [GeV]", 0., 120.);
    const HistPlotVar& MWb   = histPlot->GetNewVar("MWb", "M_{W}^{b} [GeV]", 0., 120.);
    const HistPlotVar& MTa   = histPlot->GetNewVar("MTa", "M_{t}^{a} [GeV]", 100, 250.);
    const HistPlotVar& MTb   = histPlot->GetNewVar("MTb", "M_{t}^{b} [GeV]", 100., 250.);
    const HistPlotVar& etaTa   = histPlot->GetNewVar("etaTa", "#eta_{t}^{a}", -4., 4.);
    const HistPlotVar& etaTb   = histPlot->GetNewVar("etaTb", "#eta_{t}^{b}", -4., 4.);
    const HistPlotVar& dphitaWCM  = histPlot->GetNewVar("dphitaW", "#Delta #phi_{t_{a}W}^{CM}", 0., 3.2);
    const HistPlotVar& dphillCM  = histPlot->GetNewVar("dphillCM", "#Delta #phi_{#it{l} #it{l}}^{CM}", 0., 3.2);
    const HistPlotVar& dphillLAB  = histPlot->GetNewVar("dphillLAB", "#Delta #phi_{#it{l} #it{l}}^{LAB}", 0., 3.2);
    const HistPlotVar& CMVisShape  = histPlot->GetNewVar("PPVisShape", "PPVisShape", 0., 1.);
    const HistPlotVar& dphiTT  = histPlot->GetNewVar("dphiTT", "#Delta #phi_{t_{a}t_{b}}^{CM} - #Delta #phi_{t_{a}t_{b}}^{CM,true}", -1., 1.);

    histPlot->AddPlot(MCM, cat_Reco);
    histPlot->AddPlot(dphiTT, cat_Reco);
    histPlot->AddPlot(MW, cat_Reco);
    histPlot->AddPlot(MWa, cat_Reco);
    histPlot->AddPlot(MWb, cat_Reco);
    histPlot->AddPlot(MTa, cat_Reco);
    histPlot->AddPlot(MTb, cat_Reco);
    histPlot->AddPlot(MWa, MWb, cat_Reco);
    histPlot->AddPlot(MCM, MW, cat_Reco);

    histPlot->AddPlot(MTa, MTb, cat_Reco);

    auto start = chrono::high_resolution_clock::now();
    for(int igen = 0; igen < Ngen; igen++){

        if(igen%((std::max(Ngen,10))/10) == 0)
            g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;
        // generate event
        LAB_Gen.ClearEvent();                             // clear the gen tree
        double pT = gRandom->Rndm()*100.;
        //cout << "Transverse boost: " << pT << endl;
        LAB_Gen.SetTransverseMomentum(pT);
        double pL = gRandom->Rndm()*100.;
        //cout << "Longitudinal boost: " << pL << endl;
        LAB_Gen.SetLongitudinalMomentum(pL);
        LAB_Gen.AnalyzeEvent();                           // generate a new event

        TVector3 MET = LAB_Gen.GetInvisibleMomentum();
        MET.SetZ(0.);

        LAB.ClearEvent();

        INV.SetLabFrameThreeVector(MET);

        std::vector<RFKey> B_ID;
        B_ID.push_back(COMB_B.AddLabFrameFourVector(ba_Gen.GetFourVector()));
        B_ID.push_back(COMB_B.AddLabFrameFourVector(bb_Gen.GetFourVector()));
        B_ID.push_back(COMB_B.AddLabFrameFourVector(j1a_Gen.GetFourVector()));
        B_ID.push_back(COMB_B.AddLabFrameFourVector(j2a_Gen.GetFourVector()));


        std::vector<RFKey> L_ID;
        L_ID.push_back(COMB_L.AddLabFrameFourVector(l_Gen.GetFourVector()));
        L_ID.push_back(COMB_L.AddLabFrameFourVector(lb_Gen.GetFourVector()));

        LAB.AnalyzeEvent();

        MCM = CM.GetMass()/CM_Gen.GetMass();
        MW = W.GetMass();
        MWa = Wa.GetMass();
        MWb = Wb.GetMass();
        MTa = Ta.GetMass();
        MTb = Tb.GetMass();
        TLorentzVector vCM_Ta = Ta.GetFourVector(CM);
        TLorentzVector vCM_Tb = Tb.GetFourVector(CM);
        TLorentzVector vCM_W = W.GetFourVector(CM);
        TLorentzVector vCM_l = l.GetFourVector(CM);
        TLorentzVector vCM_lb = lb.GetFourVector(CM);

        dphitaWCM = fabs(vCM_W.DeltaPhi(vCM_Ta));
        dphillCM = fabs(vCM_l.DeltaPhi(vCM_lb));
        CMVisShape = CM.GetVisibleShape();

        TLorentzVector vLAB_Gen_Ta = Ta_Gen.GetFourVector(LAB_Gen);
        TLorentzVector vLAB_Gen_Tb = Tb_Gen.GetFourVector(LAB_Gen);
        TLorentzVector vLAB_Ta = Ta.GetFourVector(LAB);
        TLorentzVector vLAB_Tb = Tb.GetFourVector(LAB);
        etaTa = vLAB_Ta.PseudoRapidity();
        etaTb = vLAB_Tb.PseudoRapidity();
        dphiTT = (vLAB_Ta.DeltaPhi(vLAB_Tb)) - (vLAB_Gen_Ta.DeltaPhi(vLAB_Gen_Tb));

        TLorentzVector vLAB_l = l.GetFourVector(LAB);
        TLorentzVector vLAB_lb = lb.GetFourVector(LAB);
        dphillLAB = fabs(vLAB_l.DeltaPhi(vLAB_lb));

        histPlot->Fill(cat_Reco);
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop-start);
    cout << "TIME TAKEN: " << duration.count() << endl;

    histPlot->Draw();
    LAB_Gen.PrintGeneratorEfficiency();

    TFile fout(output_name.c_str(),"RECREATE");
    fout.Close();
    histPlot->WriteOutput(output_name);
    histPlot->WriteHist(output_name);
    treePlot->WriteOutput(output_name);

    g_Log << LogInfo << "Finished" << LogEnd;

}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
    example_ttW();
    return 0;
}
               
