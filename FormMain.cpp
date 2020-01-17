//---------------------------------------------------------------------------

#include <vcl.h>
#include <vfw.h>
#pragma hdrstop

#include "FormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmMain *FrmMain;
//---------------------------------------------------------------------------
__fastcall TFrmMain::TFrmMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmMain::FormCreate(TObject *Sender)
{
  // load AVICAP32.DLL
	 AVICapDllHandle = LoadLibrary ( L"AVICAP32.DLL" );
  if ( AVICapDllHandle == 0 )
     {
	   MessageBox ( NULL, L"LoadLibrary:\nAVICAP32.DLL not found.",
						  L"Error FS 002", MB_ICONSTOP );
     }

  // load function address (some casting is required...)
     capCreateCaptureWindow = ( HWND ( __stdcall *)( LPCSTR,
  	                      DWORD, int, int, int, int, HWND, int ) )
        GetProcAddress ( AVICapDllHandle, "capCreateCaptureWindowA" );
     if ( capCreateCaptureWindow == NULL )
        {
		  MessageBox ( NULL, L"GetProcAddress:\ncapCreateCaptureWindow not found.",
							 L"Error FS 003", MB_ICONSTOP );
        }

 //DoubleBuffered=true;
 pnlLive->DoubleBuffered=true;
 pnlCam->DoubleBuffered=true;
 Threshold=30;
 FCapHandle=capCreateCaptureWindow("Video", WS_CHILD || WS_VISIBLE, 0, 0, PICWIDTH, PICHEIGHT, pnlLive->Handle, 0);
 //SelectSource();
 SendMessage(FCapHandle, WM_CAP_DRIVER_CONNECT,0,0L);
 SendMessage(FCapHandle, WM_CAP_DLG_VIDEOSOURCE,0,0L);
 SendMessage(FCapHandle, WM_CAP_DLG_VIDEOFORMAT,0,0L);
 TempBitmap=new Graphics::TBitmap;
 bmpFrame=new Graphics::TBitmap;

 SetProp(FCapHandle, L"FRAME", bmpFrame);
 capSetCallbackOnFrame(FCapHandle, &frameCallback);
 ImgData=new cImageData(PICWIDTH, PICHEIGHT, 4);
 FirstFrame=new cImageData(PICWIDTH, PICHEIGHT, 4);

 capGrabFrameNoStop(FCapHandle);
 //bmpFrame.SaveToFile('First.bmp');
 FirstFrame->CopyFromBitmap(bmpFrame);
 Application->OnIdle=FormOnIdle;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
  SendMessage ( FCapHandle, WM_CAP_DRIVER_DISCONNECT, 1, 0L );
  //FreeLibrary ( AVICapDllHandle );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormDestroy(TObject *Sender)
{
  if (FCapHandle !=0) {
    capDriverDisconnect(FCapHandle);
    DestroyWindow(FCapHandle);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnSelectClick(TObject *Sender)
{
  FCapHandle = capCreateCaptureWindow ( "My first capture window",
                                   WS_CHILD | WS_VISIBLE,
                                   0, 0, PICWIDTH, PICHEIGHT, pnlLive->Handle, 0 );
  SendMessage ( FCapHandle, WM_CAP_DRIVER_CONNECT, 1, 0L );
  SendMessage ( FCapHandle, WM_CAP_SET_PREVIEW, TRUE, 0L );
  SendMessage ( FCapHandle, WM_CAP_SEQUENCE, 0, 0L );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnSourceClick(TObject *Sender)
{
 capDlgVideoSource(FCapHandle);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnDisplayClick(TObject *Sender)
{
 capDlgVideoDisplay(FCapHandle);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::btnFormatClick(TObject *Sender)
{
 capDlgVideoFormat(FCapHandle);
 //ResizeWindow();
}
//---------------------------------------------------------------------------
unsigned __stdcall frameCallback(HWND hWnd, PVIDEOHDR lpVHdr)
{
 TBitmapInfo bmpInfo;
 HWND hDraw;
 unsigned Result;

 Result    = NULL;
 // Graphics.TBitmap prevents conflicts with Windows.TBitmap if the
 // Windows unit appears after Graphics in the uses clause...
 //bmpFrame  = new Graphics::TBitmap; //(GetProp(hWnd, "FRAME"));
 if (bmpFrame) {

  capGetVideoFormat(hWnd, &bmpInfo, sizeof(bmpInfo));
  bmpFrame->Width   = bmpInfo.bmiHeader.biWidth;
  bmpFrame->Height  = bmpInfo.bmiHeader.biHeight;

  hDraw   = (HWND) DrawDibOpen();
  DrawDibDraw(hDraw, bmpFrame->Canvas->Handle, 0, 0, bmpFrame->Width, bmpFrame->Height, &bmpInfo.bmiHeader,
                lpVHdr->lpData, 0, 0, bmpFrame->Width, bmpFrame->Height, 0);
  DrawDibClose(hDraw);
 }
 return Result;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::imgFrameClick(TObject *Sender)
{
  /*Graphics::TBitmap *bmpFrame;

  bmpFrame  = new Graphics::TBitmap;
  try {
    // We need a way for the callback to access our bitmap. I'm
    // using 'Window Properties' for this purpose...
    SetProp(FCapHandle, "FRAME", bmpFrame);

    capSetCallbackOnFrame(FCapHandle, frameCallback);
    capGrabFrameNoStop(FCapHandle);

    // The callback would also be called for the preview, so we're
    // resetting it here to get just one frame...
    capSetCallbackOnFrame(&FCapHandle, NULL);
    RemoveProp(&FCapHandle, "FRAME");

    // For this demo app, simply make it visible
    imgFrame->Picture->Assign(bmpFrame);
    bmpFrame->SaveToFile("Sample"+IntToStr(Count)+".bmp");
    Count++;
  } __finally {
    FreeAndNil(bmpFrame);
  }*/
  bmpFrame->SaveToFile("Sample"+IntToStr(Count)+".bmp");
  Count++;
}

/*void TFrmMain::SelectSource()
{
  int iDriver;
  TCapDriverCaps pCaps;

  iDriver = TfrmSelectSource->Execute();
  if (iDriver > -1) {
    // Connecting a new driver automatically disconnects any previous driver
    if (capDriverConnect(&FCapHandle, iDriver) ) {
      // Determine which kind of dialogs the driver supports
      memset(pCaps, 0, sizeof(pCaps));
      capDriverGetCaps(&FCapHandle, pCaps, sizeof(pCaps));

      btnSource->Enabled   = pCaps.fHasDlgVideoSource;
      btnDisplay->Enabled  = pCaps.fHasDlgVideoDisplay;
      btnFormat->Enabled   = pCaps.fHasDlgVideoFormat;

      ResizeWindow();
      capPreview(&FCapHandle, True);
      capPreviewRate(&FCapHandle, 15);
    } else
      raise Exception.Create("Unable to connect to the selected driver.");
  }
}*/

/*void TFrmMain::ResizeWindow()
{
  //TCapStatus pStatus;
  Graphics::TBitmap *bmpEmpty;

  // Query the driver for it's current image resolution
  memset(pStatus, 0, sizeof(pStatus));
  capGetStatus(&FCapHandle, pStatus, sizeof(pStatus));

  // Align and AutoSize didn't quite work out, so I'm taking the
  // ol' manual route for now...
  pnlLive->Width       = pStatus.uiImageWidth;
  pnlLive->Height      = pStatus.uiImageHeight + 20;
  imgFrame->Width      = pStatus.uiImageWidth;
  imgFrame->Height     = pStatus.uiImageHeight;
  pnlFrame->Width      = pnlLive->Width;
  pnlFrame->Height     = pnlLive->Height;
  pnlFrame->Left       = pnlLive->Width;
  pnlCam->Height       = pnlLive->Height;
  Self->ClientWidth    = pnlLive->Width + pnlFrame->Width;
  Self->ClientHeight   = pnlCam->Height + pnlButtons->Height;

  SetWindowPos(&FCapHandle, 0, 0, 0, pStatus.uiImageWidth, pStatus.uiImageHeight,
               SWP_NOACTIVATE || SWP_NOMOVE || SWP_NOZORDER ||
               SWP_NOOWNERZORDER);

  bmpEmpty  = new Graphics::TBitmap;
  try {
    bmpEmpty->Width              = imgFrame->Width;
    bmpEmpty->Height             = imgFrame->Height;
    bmpEmpty->Canvas.Brush.Color = clBlack;
    bmpEmpty->Canvas->FillRect(Rect(0, 0, bmpEmpty->Width, bmpEmpty->Height));

    imgFrame->Picture->Assign(bmpEmpty);
  } __finally {
    FreeAndNil(bmpEmpty);
  }
}*/

void __fastcall TFrmMain::FormOnIdle(TObject *Sender, bool &Done)
{
 unsigned char RF, RP, GF, GP, BF, BP, AF, AP;
 short int DR, DG, DB;
 bool Difference;
 int x, y;

 //SendMessage(FCapHandle,CWM_CAP_GET_FRAME, 0, 0);
 capGrabFrameNoStop(FCapHandle);
 ImgSrc->Picture->Bitmap->Assign(bmpFrame);
 ImgData->CopyFromBitmap(bmpFrame);

 Difference=false;
 for(y=0; y<ImgData->Height; y++) {
  for(x=0; x<ImgData->Width; x++) {
   ImgData->GetARGB(x, y, &AP, &RP, &GP, &BP);
   FirstFrame->GetARGB(x, y, &AF, &RF, &GF, &BF);
   DR=abs( ((short)RF)-RP );
   DG=abs( ((short)GF)-GP );
   DB=abs( ((short)BF)-BP );

   if ( DR>Threshold || DG>Threshold || DB>Threshold) {
    Difference=true;
    //ImgProc->SetARGB(x, y, 255, 255, 0, 0);
   } else ImgData->SetARGB(x, y, 255, 255, 255, 255);
  }
 } /*int x, y, Color, tColor;
 unsigned char A, R, G, B;
 for(y=0; y<ImgData->Height; y++) {
  z=ImgData->Width-1;
  for(x=0; x<ImgData->Width/2; x++) {
   ImgData->GetRGB(x, y, &R, &G, &B);
   ImgData->SetPixel(x, y, ImgData, z, y);
   ImgData->SetRGB(z, y, R, G, B);
   z--;
  }
 }*/
 ImgData->CopyToBitmap(TempBitmap, pf32bit);

 imgFrame->Picture->Bitmap->Assign(TempBitmap);
}

//---------------------------------------------------------------------------

