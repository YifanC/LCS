//#include <TRootEmbeddedCanvas.h>
#include "GUItest.h"
#include "TFile.h"
#include "TThread.h"
#include "TCanvas.h"
#include "TH1F.h"
#include <TButton.h>
#include "TGButton.h" 
#include "TGButtonGroup.h"
#include "TGCanvas.h"
//#include <qlcdnumber.h>

ClassImp(MyMainFrame)

//extern void hello(int n);
extern TThread        *th1;
extern TThread        *th2;
extern MyMainFrame    *Frame;
extern Pixel_t color, color1;
//extern struct livello INPUT[16];
//extern TH1F                    *h_low[16], *h_high[16];

  using namespace std;


MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h,const Int_t NFILE) : TGMainFrame(p,w,h)
{

// inside the window you can have space managers, horizontal or vertical frames
// here is a horizontal (all members will be side be side)
// this stands for the object of this class and means the parent Widget 
//Frame->SetWindowName("LNGS Warp Experiment DAQ Monitoring Window");

  TGHorizontalFrame *hframe      = new TGHorizontalFrame(this,300,300);
  TGVerticalFrame   *vframe_ch04 = new TGVerticalFrame(hframe,300,300);
  TGVerticalFrame   *vframe_ch15 = new TGVerticalFrame(hframe,300,300);
  TGVerticalFrame   *vframe_ch26 = new TGVerticalFrame(hframe,300,300);
  TGVerticalFrame   *vframe_ch37 = new TGVerticalFrame(hframe,300,300);
  TGVerticalFrame   *vframe_but  = new TGVerticalFrame(hframe,300,300);

  
  //create two embedded canvases of which hframe is the parent widget
  fEcanvas0 = new TRootEmbeddedCanvas("ECanvas0",vframe_ch04,250,250);
  fEcanvas1 = new TRootEmbeddedCanvas("ECanvas1",vframe_ch15,250,250); 
  fEcanvas2 = new TRootEmbeddedCanvas("ECanvas2",vframe_ch26,250,250);
  fEcanvas3 = new TRootEmbeddedCanvas("ECanvas3",vframe_ch37,250,250); 
  fEcanvas4 = new TRootEmbeddedCanvas("ECanvas4",vframe_ch04,250,250); 
  fEcanvas5 = new TRootEmbeddedCanvas("ECanvas5",vframe_ch15,250,250); 
  fEcanvas6 = new TRootEmbeddedCanvas("ECanvas6",vframe_ch26,250,250); 
  fEcanvas7 = new TRootEmbeddedCanvas("ECanvas7",vframe_ch37,250,250); 
  
  //   Int_t wid = fEcanvas1->GetCanvasWindowId();
  //   TCanvas *myc = new TCanvas("myname", 10, 10, wid);
  //   fEcanvas1->AdoptCanvas(myc);
 
  //connect the canvases to the horizontal frame.
  vframe_ch04->AddFrame(fEcanvas0,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ch15->AddFrame(fEcanvas1,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ch26->AddFrame(fEcanvas2,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ch37->AddFrame(fEcanvas3,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ch04->AddFrame(fEcanvas4,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ch15->AddFrame(fEcanvas5,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ch26->AddFrame(fEcanvas6,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
  vframe_ch37->AddFrame(fEcanvas7,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,10,10,10,1));
//create text button, hello is the name on the button.
// int Color_num = 42; 
 
 TGVButtonGroup *bg = new TGVButtonGroup(vframe_but); 
 bg->SetTitle("Show Board");

 vframe_but->SetBackgroundColor(color);
 hframe->SetBackgroundColor(color);
 bg->SetBackgroundColor(color);
 // quit->SetBackgroundColor(color1);
 
 TGRadioButton *b0   = new TGRadioButton(bg,"Board &0");
 TGRadioButton *b1   = new TGRadioButton(bg,"Board &1");
 TGRadioButton *b2   = new TGRadioButton(bg,"Board &2");
 TGRadioButton *b3   = new TGRadioButton(bg,"Board &3");

 // create legend for hovering over the button
 b0->SetToolTipText("Set the view on the board 0");
 b1->SetToolTipText("Set the view on the board 1");
 b2->SetToolTipText("Set the view on the board 2");
 b3->SetToolTipText("Set the view on the board 3");

 //b0->SetForegroundColor(11);
 b0->SetTextColor(8,true);
 b1->SetTextColor(color,true); 
 b2->SetTextColor(5,true);



 //QLCDNumber *LCD = new QLCDNumber(vframe, "LCD written run");
 //LCD->setNumDigits ( 6 ); 
 
 TGTextButton *quit = new TGTextButton(vframe_but," &quit ", "gApplication->Terminate()");
 quit->SetTextColor(0xFF00FF,true);
 quit->SetToolTipText("Quit the application");

 bg->Show();    
 // Set Default Channel to Show in fEcanvas2 ( right canvas )
 bg->SetButton(1, true);

 //b0->SetFillColor(Color_num);

 //connect the action "clicked" to a method of class MyMainFrame, the current object, method name hello with value =1
 
  b0->Connect("Clicked()","MyMainFrame",bg,"Ch_Histo(=0)");
  b1->Connect("Clicked()","MyMainFrame",bg,"Ch_Histo(=1)");
  b2->Connect("Clicked()","MyMainFrame",bg,"Ch_Histo(=2)");
  b3->Connect("Clicked()","MyMainFrame",bg,"Ch_Histo(=3)");
  //  quit->Connect("Clicked()","theApp",this,"quit");

  //connect button to hframe. 
  vframe_but->AddFrame(bg, new TGLayoutHints(kLHintsRight,5,5,3,4));

  hframe->AddFrame(vframe_ch37,new TGLayoutHints(kLHintsRight,5,5,3,4));
  hframe->AddFrame(vframe_ch26,new TGLayoutHints(kLHintsRight,5,5,3,4));
  hframe->AddFrame(vframe_ch15,new TGLayoutHints(kLHintsRight,5,5,3,4));
  hframe->AddFrame(vframe_ch04,new TGLayoutHints(kLHintsRight,5,5,3,4));
  hframe->AddFrame(vframe_but, new TGLayoutHints(kLHintsRight,5,5,3,4));

  vframe_but->AddFrame(quit, new TGLayoutHints(kLHintsRight,5,5,3,4));
 
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

void MyMainFrame::Ch_Histo(int n)
{
  n_histo = n;
  cout << "n equals " << n << endl;
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
