//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Windows.hpp>
#include <Graphics.hpp>
#include <SysUtils.hpp>
#include "ImageData.h"

/*#define CWM_USER                  1024
#define CWM_CAP_DRIVER_CONNECT    1034
#define CWM_CAP_DRIVER_DISCONNECT 1035
#define CWM_CAP_GET_FRAME         1084
#define CWM_CAP_COPY              1054

#define CWM_CAP_START                WM_USER
#define CWM_CAP_DLG_VIDEOFORMAT      WM_CAP_START + 41
#define CWM_CAP_DLG_VIDEOSOURCE      WM_CAP_START + 42
#define CWM_CAP_DLG_VIDEODISPLAY     WM_CAP_START + 43
#define CWM_CAP_GET_VIDEOFORMAT      WM_CAP_START + 44
#define CWM_CAP_SET_VIDEOFORMAT      WM_CAP_START + 45
#define CWM_CAP_DLG_VIDEOCOMPRESSION WM_CAP_START + 46
#define CWM_CAP_SET_PREVIEW          WM_CAP_START + 50*/

#define PICWIDTH  640
#define PICHEIGHT 480

Graphics::TBitmap *bmpFrame;
unsigned __stdcall frameCallback(HWND hWnd, PVIDEOHDR lpVHdr);
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnlButtons;
        TButton *btnSelect;
        TButton *btnDisplay;
        TButton *btnFormat;
        TButton *btnSource;
        TPanel *pnlCam;
        TPanel *pnlLive;
        TPanel *pnlFrame;
        TImage *imgFrame;
        TImage *ImgSrc;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall btnSelectClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall btnSourceClick(TObject *Sender);
        void __fastcall btnDisplayClick(TObject *Sender);
        void __fastcall btnFormatClick(TObject *Sender);
        void __fastcall imgFrameClick(TObject *Sender);
private:	// User declarations
        //THandle FCapWindow;
        HWND FCapHandle;
        Graphics::TBitmap *TempBitmap;
        short int Count, Threshold;
        cImageData *ImgData, *FirstFrame;

        HINSTANCE AVICapDllHandle;
        HWND ( __stdcall *capCreateCaptureWindow )( LPCSTR, DWORD, int, int, int, int, HWND, int );

        /*void SelectSource();
        void ResizeWindow();*/
        void __fastcall FormOnIdle(TObject *Sender, bool &Done);
public:		// User declarations
        __fastcall TFrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------

#endif
