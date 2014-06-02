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


 void Ch_Histo(int n);

 void quit();

 static int n_histo;

 TRootEmbeddedCanvas* fEcanvas0;
 TRootEmbeddedCanvas* fEcanvas1;
 TRootEmbeddedCanvas* fEcanvas2;
 TRootEmbeddedCanvas* fEcanvas3;
 TRootEmbeddedCanvas* fEcanvas4;
 TRootEmbeddedCanvas* fEcanvas5;
 TRootEmbeddedCanvas* fEcanvas6;
 TRootEmbeddedCanvas* fEcanvas7;
 
 protected:
  

  ClassDef(MyMainFrame,1);
 };




#endif


 


