// reading a text file
#include <iostream>
#include <fstream>
#include <string>

// ROOT includes
#include "TApplication.h"
#include "TThread.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TStyle.h"

using namespace std;

int main (int argc,char** argv) {
  
  TH1F* hf[5] = {0};
  char h_name[128] = {0};
  
  for(Int_t i=0; i<5;i++){
    sprintf(h_name, "histo_%i", i);
    hf[i] = new TH1F(h_name,"Titolo",50, 0.7, 2.7);
  }
  
  float  value[5]  = {0.};
  int bin       = 0;
  string line;
  ifstream myfile1 ("Calib001.txt");
  ifstream myfile2 ("Calib002.txt");
  ifstream myfile3 ("Calib003.txt");
  ifstream myfile4 ("Calib004.txt");
  ifstream myfile5 ("Calib005.txt");
  
  if (myfile1.is_open()){
    do{
      getline (myfile1,line);
      myfile1 >> bin >> value[0];
      getline (myfile2,line);
      myfile2 >> bin >> value[1];
      getline (myfile3,line);
      myfile3 >> bin >> value[2];
      getline (myfile4,line);
      myfile4 >> bin >> value[3];
      getline (myfile5,line);
      myfile5 >> bin >> value[4];
      // cout << bin << "   " << value << endl;
      hf[0]->Fill(value[0]);
      hf[1]->Fill(value[1]);
      hf[2]->Fill(value[2]);
      hf[3]->Fill(value[3]);
      hf[4]->Fill(value[4]);

    }while(! myfile1.eof() );
  
    myfile1.close();
    myfile2.close();
    myfile3.close();
    myfile4.close();
    myfile5.close();

  } else cout << "Unable to open file"; 
  
  TApplication theApp("h1Thread", &argc, argv); 
  TCanvas* c1 = new TCanvas("c1","Geant4 histo",900,600);
  TF1 *f1 = new TF1("f1", "gaus", 0.5, 3);
  
  Double_t chi2[5] = {0};
  Double_t ndf[5]  = {0};
  Double_t par[5]  = {0};//value of 1st parameter
  Double_t sig[5]  = {0};//value of 1st parameter
  Double_t err[5]  = {0};//error on first parameter
  
  // Canvas parts
  c1->Divide(2,3); 
  for(Int_t i=0;i<5;i++){
    c1->cd(i+1);
    hf[i]->Draw();
    hf[i]->Fit("f1", "R");
    chi2[i] = f1->GetChisquare();
    ndf[i] = f1->GetNDF();
    par[i]  = f1->GetParameter("Mean"); //value of 1st parameter
    sig[i]  = f1->GetParameter("Sigma");
    //err[i]  = f1->GetParError("Mean"); 
  }
  
  for(Int_t i=0;i<5;i++)cout << "Chi2= "<<chi2[i]/ndf[i] << "  Mean= " << par[i] << "  Sigma= "<<sig[i]<<  "  Ratio= " << par[i]/par[0]  <<endl;
  
  



  theApp.Run(kTRUE);

  return 0;
}
