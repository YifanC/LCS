#ifndef __MY_GUI__
#define __MY_GUI__
#include <TGFrame.h>
#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <iostream>



class TRootEmbeddedCanvas;
class TGWindow;
class TGMainFrame;
class TCanvas;
class MyMainFrame : public TGMainFrame {

 private:



 public:
  MyMainFrame(const TGWindow *p,UInt_t w, UInt_t h,Int_t NFILE);
  
  virtual ~MyMainFrame();


  // void W_up();
  // void W_down();
 void IBoard_select(int i);
 void CBoard_select(int i);
 void quit();

 static int n_histo;
 static int I_board;
 static int C_board;
 static int event;

 TRootEmbeddedCanvas* canv_ind;
 TRootEmbeddedCanvas* canv_w_ind0;
 TRootEmbeddedCanvas* canv_col;
 TRootEmbeddedCanvas* canv_w_col0;
 // TRootEmbeddedCanvas* canv_w_col1;
 // TRootEmbeddedCanvas* canv_w_col2;
 // TRootEmbeddedCanvas* canv_w_col3;

protected:
  

  ClassDef(MyMainFrame,1);
 };




#endif


 


