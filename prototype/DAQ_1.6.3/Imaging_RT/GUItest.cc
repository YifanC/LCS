#include "GUItest.h"
#include "TFile.h"
#include "TThread.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <TButton.h>
#include "TGButton.h" 
#include "TGButtonGroup.h"
#include "TGCanvas.h"
#include "TText.h"

#include "../daq_main.h"

ClassImp(MyMainFrame)

#define MAX_WIRE N_BOARD * N_CHANNEL
#define MIN_WIRE 1

extern TThread        *th1;
extern TThread        *th2;
extern MyMainFrame    *Frame;
extern Pixel_t         color, color1;

  using namespace std;


MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h,const Int_t NFILE) : TGMainFrame(p,w,h)
{

  Int_t width  =  500;
  Int_t height =  400;
  // inside the window you can have space managers, horizontal or vertical frames
  // here is a horizontal (all members will be side be side)
  // this stands for the object of this class and means the parent Widget 
  TGHorizontalFrame *hframe        = new TGHorizontalFrame(this, width, height);
  TGVerticalFrame   *vframe_butI   = new TGVerticalFrame(hframe,100,100);
  TGVerticalFrame   *vframe_ind    = new TGVerticalFrame(hframe, width,height);
  TGVerticalFrame   *vframe_col    = new TGVerticalFrame(hframe, width,height);
  //TGVerticalFrame   *vframe_wires  = new TGVerticalFrame(vframe_col,500,800);
  TGVerticalFrame   *vframe_butC   = new TGVerticalFrame(hframe,100,100);
  
  //create two embedded canvases of which hframe is the parent widget
  canv_ind    = new TRootEmbeddedCanvas("Ind ",       vframe_ind,  width,height);
  canv_w_ind0 = new TRootEmbeddedCanvas("Wire 0 ind ",vframe_ind,  width,height/2);
  canv_col    = new TRootEmbeddedCanvas("Coll",       vframe_col,  width,height);
  canv_w_col0 = new TRootEmbeddedCanvas("Wire 0 coll",vframe_col,  width,height/2);
  //canv_w_col1 = new TRootEmbeddedCanvas("Wire 1 coll",vframe_wires,500,190);
  //canv_w_col2 = new TRootEmbeddedCanvas("Wire 2 coll",vframe_wires,500,190);
  //canv_w_col3 = new TRootEmbeddedCanvas("Wire 3 coll",vframe_wires,500,190);

  //connect the canvases to the horizontal frame.
  vframe_ind->AddFrame(canv_ind,     new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ind->AddFrame(canv_w_ind0,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_col->AddFrame(canv_col,     new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_col->AddFrame(canv_w_col0,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  // vframe_col->AddFrame(vframe_wires, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  //vframe_wires->AddFrame(canv_w_col1,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  //vframe_wires->AddFrame(canv_w_col2,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  //vframe_wires->AddFrame(canv_w_col3,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));

  vframe_ind->SetBackgroundColor(color);
  vframe_col->SetBackgroundColor(color);
  // vframe_wires->SetBackgroundColor(color);
  vframe_butI->SetBackgroundColor(color);
  vframe_butC->SetBackgroundColor(color);
  hframe->SetBackgroundColor(color);
 
  //  TGTextButton  *b0    = new TGTextButton( vframe_but,"Wire 5,6,7,8");
  //TGTextButton  *b1    = new TGTextButton( vframe_but,"Wire 1,2,3,4");
  
  TGVButtonGroup *ind1 = new TGVButtonGroup(vframe_butI, "Induction 1");
  TGVButtonGroup *ind2 = new TGVButtonGroup(vframe_butI, "Induction 2");
  TGVButtonGroup *col1 = new TGVButtonGroup(vframe_butC, "Collection 1");
  TGVButtonGroup *col2 = new TGVButtonGroup(vframe_butC, "Collection 2");
  
  ind1->SetBackgroundColor(color);
  ind2->SetBackgroundColor(color);
  col1->SetBackgroundColor(color);
  col2->SetBackgroundColor(color);

  TGRadioButton  *rb_00 = new TGRadioButton(ind1, "Ind   0- 7");
  TGRadioButton  *rb_01 = new TGRadioButton(ind1, "Ind   8-15");
  TGRadioButton  *rb_02 = new TGRadioButton(ind1, "Ind  16-23");
  TGRadioButton  *rb_03 = new TGRadioButton(ind1, "Ind  24-31");
  TGRadioButton  *rb_04 = new TGRadioButton(ind2, "Ind  32-39");
  TGRadioButton  *rb_05 = new TGRadioButton(ind2, "Ind  40-47");
  TGRadioButton  *rb_06 = new TGRadioButton(ind2, "Ind  48-55");
  TGRadioButton  *rb_07 = new TGRadioButton(ind2, "Ind  56-63");
  TGRadioButton  *rb_08 = new TGRadioButton(col1, "Col   0- 7");
  TGRadioButton  *rb_09 = new TGRadioButton(col1, "Col   8-15");
  TGRadioButton  *rb_10 = new TGRadioButton(col1, "Col  16-23");
  TGRadioButton  *rb_11 = new TGRadioButton(col1, "Col  24-31");
  TGRadioButton  *rb_12 = new TGRadioButton(col2, "Col  32-39");
  TGRadioButton  *rb_13 = new TGRadioButton(col2, "Col  40-47");
  TGRadioButton  *rb_14 = new TGRadioButton(col2, "Col  48-55");
  TGRadioButton  *rb_15 = new TGRadioButton(col2, "Col  56-63");
  
  //TGTextButton   *b3    = new TGTextButton( vframe_but,"ADC counts");
  TGTextButton   *quit  = new TGTextButton( vframe_butC," &quit ", "gApplication->Terminate()");
  
  rb_00->SetBackgroundColor(color);
  rb_01->SetBackgroundColor(color);
  rb_02->SetBackgroundColor(color);
  rb_03->SetBackgroundColor(color);
  rb_04->SetBackgroundColor(color);
  rb_05->SetBackgroundColor(color);
  rb_06->SetBackgroundColor(color);
  rb_07->SetBackgroundColor(color);
  rb_08->SetBackgroundColor(color);
  rb_09->SetBackgroundColor(color);
  rb_10->SetBackgroundColor(color);
  rb_11->SetBackgroundColor(color);
  rb_12->SetBackgroundColor(color);
  rb_13->SetBackgroundColor(color);
  rb_14->SetBackgroundColor(color);
  rb_15->SetBackgroundColor(color);

  rb_00->Connect("Clicked()","MyMainFrame",ind1,"IBoard_select(= 0)");
  rb_01->Connect("Clicked()","MyMainFrame",ind1,"IBoard_select(= 1)");
  rb_02->Connect("Clicked()","MyMainFrame",ind1,"IBoard_select(= 2)");
  rb_03->Connect("Clicked()","MyMainFrame",ind1,"IBoard_select(= 3)");
  rb_04->Connect("Clicked()","MyMainFrame",ind2,"IBoard_select(= 4)");
  rb_05->Connect("Clicked()","MyMainFrame",ind2,"IBoard_select(= 5)");
  rb_06->Connect("Clicked()","MyMainFrame",ind2,"IBoard_select(= 6)");
  rb_07->Connect("Clicked()","MyMainFrame",ind2,"IBoard_select(= 7)");
  rb_08->Connect("Clicked()","MyMainFrame",col1,"CBoard_select(= 8)");
  rb_09->Connect("Clicked()","MyMainFrame",col1,"CBoard_select(= 9)");
  rb_10->Connect("Clicked()","MyMainFrame",col1,"CBoard_select(=10)");
  rb_11->Connect("Clicked()","MyMainFrame",col1,"CBoard_select(=11)");
  rb_12->Connect("Clicked()","MyMainFrame",col2,"CBoard_select(=12)");
  rb_13->Connect("Clicked()","MyMainFrame",col2,"CBoard_select(=13)");
  rb_14->Connect("Clicked()","MyMainFrame",col2,"CBoard_select(=14)");
  rb_15->Connect("Clicked()","MyMainFrame",col2,"CBoard_select(=15)");

  ind1->SetButton(1, true);
  rb_00->SetTextColor(2,true);
  col1->SetButton(1, true);
  rb_08->SetTextColor(2,true);
 
  //connect button to vframe_but.
  vframe_butI->AddFrame(ind1,   new TGLayoutHints(kLHintsRight,5,5,3,4));
  vframe_butI->AddFrame(ind2,   new TGLayoutHints(kLHintsRight,5,5,3,4));
  vframe_butC->AddFrame(col1,   new TGLayoutHints(kLHintsRight,5,5,3,4));
  vframe_butC->AddFrame(col2,   new TGLayoutHints(kLHintsRight,5,5,3,4));
  vframe_butC->AddFrame(quit,   new TGLayoutHints(kLHintsRight,5,5,3,4));
  
  hframe->AddFrame(vframe_butC, new TGLayoutHints(kLHintsRight,5,5,3,4));
  hframe->AddFrame(vframe_col, new TGLayoutHints(kLHintsRight,5,5,3,4));
  hframe->AddFrame(vframe_ind, new TGLayoutHints(kLHintsRight,5,5,3,4));
  hframe->AddFrame(vframe_butI, new TGLayoutHints(kLHintsRight,5,5,3,4));
  // connect hframe to the MyMainFrame current object
  AddFrame(hframe, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));

  //draw the thing
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
  
} // end of constructor.

MyMainFrame::~MyMainFrame()
{
  
}

void MyMainFrame::IBoard_select(int i)
{
  I_board = i;
  //cout << "IBoard_nr =" << I_board << endl;
}

void MyMainFrame::CBoard_select(int i)
{
  C_board = i;
  //cout << "C Board_nr =" << C_board << endl;
}
extern bool BIAGIO;

void MyMainFrame::quit()
{
  th1->SetCancelAsynchronous();
  
  th1->Kill();
  
  printf("Threads END...\n");
  
 //  TFile f("br.root","RECREATE"); // Crea il file "br.root"
//   for(int k=0; k<16; k++)
//      {
//        h_low[k]->Write();
//        h_high[k]->Write();
//      }
  
//  write_file_txt();
    
  printf("\n Filling Section End \n");
  
  //  f.Close();
 
  gApplication->Terminate();

}
