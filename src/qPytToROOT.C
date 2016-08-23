//c headers
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

//ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

//include headers
#include "include/checkMakeDir.h"
#include "include/getPartsOfString.h"
#include "include/strToFloat.h"


//Fastjet headers                                                                                                                                                         
#include "fastjet/ClusterSequence.hh"

const std::string txtStr = ".txt";
const bool debug = false;

const Int_t nJtR = 4;
const Double_t jtR[nJtR] = {0.2, 0.3, 0.4, 0.5};

const fastjet::JetAlgorithm jtAlg = fastjet::antikt_algorithm;

const fastjet::JetDefinition jtDef2(jtAlg, jtR[0], fastjet::E_scheme, fastjet::Best);
const fastjet::JetDefinition jtDef3(jtAlg, jtR[1], fastjet::E_scheme, fastjet::Best);
const fastjet::JetDefinition jtDef4(jtAlg, jtR[2], fastjet::E_scheme, fastjet::Best);
const fastjet::JetDefinition jtDef5(jtAlg, jtR[3], fastjet::E_scheme, fastjet::Best);

const fastjet::JetDefinition jtDef[nJtR] = {jtDef2, jtDef3, jtDef4, jtDef5};

int qPytToROOT(const std::string inFileName)
{
  if(!checkFile(inFileName)){
    std::cout << "Input \'" << inFileName << "\' is not valid file. return 1" << std::endl;
    return 1;
  }
  if(inFileName.substr(inFileName.size()-txtStr.size(), txtStr.size()).find(txtStr) != std::string::npos){
    std::cout << "Input \'" << inFileName << "\' is not extension \'" << txtStr << "\'. return 1" << std::endl;
    return 1;
  }

  checkMakeDir("out/outRoot");

  TFile* outFile_p = new TFile("out/outRoot/test.root", "UPDATE");
  TTree* outTree_p = new TTree("outTree_p", "outTree_p");

  const Int_t nMaxPart = 100000;
  Int_t nPart = 0;
  Float_t pt[nMaxPart];
  Float_t phi[nMaxPart];
  Float_t eta[nMaxPart];
  Float_t mass1[nMaxPart];
  Float_t mass2[nMaxPart];
  Int_t pdgID[nMaxPart];
  Int_t sta[nMaxPart];

  std::vector<fastjet::PseudoJet> particles;

  const Int_t nMaxJt = 500;

  Int_t nJt[nJtR];
  Float_t jtPt[nJtR][nMaxJt];
  Float_t jtPhi[nJtR][nMaxJt];
  Float_t jtEta[nJtR][nMaxJt];
  Float_t jtE[nJtR][nMaxJt];

  for(Int_t rIter = 0; rIter < nJtR; rIter++){
    nJt[rIter] = 0;
  }

  outTree_p->Branch("nPart", &nPart, "nPart/I");
  outTree_p->Branch("pt", pt, "pt[nPart]/F");
  outTree_p->Branch("phi", phi, "phi[nPart]/F");
  outTree_p->Branch("eta", eta, "eta[nPart]/F");
  outTree_p->Branch("pdgID", pdgID, "pdgID[nPart]/I");
  outTree_p->Branch("sta", sta, "sta[nPart]/I");

  outTree_p->Branch("mass1", mass1, "mass1[nPart]/F");
  outTree_p->Branch("mass2", mass2, "mass2[nPart]/F");

  for(Int_t rIter = 0; rIter < nJtR; rIter++){
    std::string rStr = std::to_string(jtR[rIter]);
    rStr = rStr.substr(rStr.find(".")+1, 1);

    rStr = "R" + rStr;

    std::string nStr1 = "nJt" + rStr;
    std::string nStr2 = "nJt" + rStr + "/I";

    std::string ptStr1 = "jt" + rStr + "Pt";
    std::string ptStr2 = "jt" + rStr + "Pt" + "[" +  nStr1 + "]/F";
    std::string phiStr1 = "jt" + rStr + "Phi";
    std::string phiStr2 = "jt" + rStr + "Phi" + "[" +  nStr1 + "]/F";
    std::string etaStr1 = "jt" + rStr + "Eta";
    std::string etaStr2 = "jt" + rStr + "Eta" + "[" +  nStr1 + "]/F";
    std::string eStr1 = "jt" + rStr + "E";
    std::string eStr2 = "jt" + rStr + "E" + "[" +  nStr1 + "]/F";

    outTree_p->Branch(nStr1.c_str(), &nJt[rIter], nStr2.c_str());
    outTree_p->Branch(ptStr1.c_str(), jtPt[rIter], ptStr2.c_str());
    outTree_p->Branch(phiStr1.c_str(), jtPhi[rIter], phiStr2.c_str());
    outTree_p->Branch(etaStr1.c_str(), jtEta[rIter], etaStr2.c_str());
    outTree_p->Branch(eStr1.c_str(), jtE[rIter], eStr2.c_str());
  }

  std::ifstream file(inFileName.c_str());
  std::string tempStr1 = "";
  std::string tempStr2 = "";
  std::vector<std::string>* tempStrParts1_p = new std::vector<std::string>;
  std::vector<std::string>* tempStrParts2_p = new std::vector<std::string>;

  Int_t currentEvent = -1;

  while(std::getline(file, tempStr1)){
    std::getline(file, tempStr2);

    getPartsOfString(tempStr1, tempStrParts1_p);
    getPartsOfString(tempStr2, tempStrParts2_p);

    Int_t tempEvent = std::stoi(tempStrParts1_p->at(0));

    if(tempEvent != currentEvent){
      for(Int_t rIter = 0; rIter < nJtR; rIter++){
	fastjet::ClusterSequence cs(particles, jtDef[rIter]);
	std::vector<fastjet::PseudoJet> jets = fastjet::sorted_by_pt(cs.inclusive_jets());

	for(Int_t jtIter = 0; jtIter < (Int_t)jets.size(); jtIter++){
	  if(jets.at(jtIter).pt() < 0) break;

	  jtPt[rIter][nJt[rIter]] = jets.at(jtIter).pt();
	  jtPhi[rIter][nJt[rIter]] = jets.at(jtIter).phi_std();
	  jtEta[rIter][nJt[rIter]] = jets.at(jtIter).eta();
	  jtE[rIter][nJt[rIter]] = jets.at(jtIter).E();

	  nJt[rIter]++;
	}
	
	jets.clear();
      }

      if(currentEvent != -1) outTree_p->Fill();
      currentEvent = tempEvent;
      nPart = 0;

      for(Int_t rIter = 0; rIter < nJtR; rIter++){
	nJt[rIter] = 0;
      }

      particles.clear();
    }

    std::string pxStr = tempStrParts2_p->at(0);
    std::string pyStr = tempStrParts2_p->at(1);
    std::string pzStr = tempStrParts2_p->at(2);
    std::string eStr = tempStrParts2_p->at(3);
    std::string mStr = tempStrParts2_p->at(4);
    

    if(debug) std::cout << __LINE__ << std::endl;

    float px = strToFloat(pxStr);
    float py = strToFloat(pyStr);
    float pz = strToFloat(pzStr);
    float e = strToFloat(eStr);
    float m = strToFloat(mStr);

    if(debug) std::cout << __LINE__ << std::endl;

    TLorentzVector part;
    part.SetPxPyPzE(px, py, pz, e);

    pt[nPart] = part.Pt();
    phi[nPart] = part.Phi();
    eta[nPart] = part.Eta();
    pdgID[nPart] = std::stoi(tempStrParts1_p->at(1));
    sta[nPart] = std::stoi(tempStrParts1_p->at(2));

    mass1[nPart] = part.M();
    mass2[nPart] = m;

    particles.push_back(fastjet::PseudoJet(px, py, pz, e));

    nPart++;

    tempStrParts1_p->clear();
    tempStrParts2_p->clear();
  }

  for(Int_t rIter = 0; rIter < nJtR; rIter++){
    fastjet::ClusterSequence cs(particles, jtDef[rIter]);
    std::vector<fastjet::PseudoJet> jets = fastjet::sorted_by_pt(cs.inclusive_jets());
    
    for(Int_t jtIter = 0; jtIter < (Int_t)jets.size(); jtIter++){
      if(jets.at(jtIter).pt() < 30) break;
      
      jtPt[rIter][nJt[rIter]] = jets.at(jtIter).pt();
      jtPhi[rIter][nJt[rIter]] = jets.at(jtIter).phi_std();
      jtEta[rIter][nJt[rIter]] = jets.at(jtIter).eta();
      jtE[rIter][nJt[rIter]] = jets.at(jtIter).E();
      
      nJt[rIter]++;
    }
    
    jets.clear();
  }

  particles.clear();

  outTree_p->Fill();

  outFile_p->cd();
  outTree_p->Write("", TObject::kOverwrite);

  tempStrParts1_p->clear();
  tempStrParts2_p->clear();

  delete tempStrParts1_p;
  delete tempStrParts2_p;

  delete outTree_p;
  outFile_p->Close();
  delete outFile_p;

  return 0;
}

int main(int argc, char *argv[])
{
  if(argc != 2){
    std::cout << "Usage: qPytToROOT.exe <inputFile>" << std::endl;
    std::cout << "Number of args given: " << argc << std::endl;
    for(int iter = 0; iter < argc; iter++){
      std::cout << "  argv[" << iter << "]: " << argv[iter] << std::endl;
    }
    return -1;
  }

  int result = qPytToROOT(argv[1]);

  return result;
}
