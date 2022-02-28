#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM , LPARAM );
void AddMenus(HWND);
void start_paint(HWND);
void registerDialogClass(HINSTANCE);
void displayDialog(HWND);
static bool draw = false;
static bool draw2 = false;
TCHAR buff[128];
	TCHAR buff2[128];
	TCHAR buff3[128];
HMENU hMenu;
HWND button1;
HWND dialog{};
HINSTANCE g_hinst;
HINSTANCE hInst;
COLORREF g_color;
HINSTANCE Instanz;
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("DigClock") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = MAKEINTRESOURCE(IDM_MODE_MENU ) ;
     wndclass.lpszClassName = szAppName ;
     Instanz=hInstance;
         hInst=hInstance;
   // InitCommonControls();

     if (!RegisterClass (&wndclass))
     {

          MessageBox (NULL, TEXT ("Program requires Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }

     hwnd = CreateWindow (szAppName, TEXT ("Digital Clock"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;

     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return msg.wParam ;
     }

void DisplayDigit (HDC hdc, int iNumber)
{
     static BOOL  fSevenSegment [10][7] = {
                         1, 1, 1, 0, 1, 1, 1,     // 0
                         0, 0, 1, 0, 0, 1, 0,     // 1
                         1, 0, 1, 1, 1, 0, 1,     // 2
                         1, 0, 1, 1, 0, 1, 1,     // 3
                         0, 1, 1, 1, 0, 1, 0,     // 4
                         1, 1, 0, 1, 0, 1, 1,     // 5
                         1, 1, 0, 1, 1, 1, 1,     // 6
                         1, 0, 1, 0, 0, 1, 0,     // 7
                         1, 1, 1, 1, 1, 1, 1,     // 8
                         1, 1, 1, 1, 0, 1, 1 } ;  // 9
     static POINT ptSegment [7][6] = {
                          7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
                          6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
                         36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
                          7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
                          6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
                         36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
                          7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70 } ;
     int          iSeg ;

     for (iSeg = 0 ; iSeg < 7 ; iSeg++)
          if (fSevenSegment [iNumber][iSeg])
               Polygon (hdc, ptSegment [iSeg], 6) ;
}

void DisplayTwoDigits (HDC hdc, int iNumber, BOOL fSuppress)
{
     if (!fSuppress || (iNumber / 10 != 0))
          DisplayDigit (hdc, iNumber / 10) ;
     OffsetWindowOrgEx (hdc, -42, 0, NULL) ;
     DisplayDigit (hdc, iNumber % 10) ;
     OffsetWindowOrgEx (hdc, -42, 0, NULL) ;
}

void DisplayColon (HDC hdc)
{
     POINT ptColon [2][4] = { 2,  21,  6,  17,  10, 21,  6, 25,
                              2,  51,  6,  47,  10, 51,  6, 55 } ;

     Polygon (hdc, ptColon [0], 4) ;
     Polygon (hdc, ptColon [1], 4) ;

     OffsetWindowOrgEx (hdc, -12, 0, NULL) ;
}

void DisplayTime (HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
     SYSTEMTIME st ;

     GetLocalTime (&st) ;

     if (f24Hour)
          DisplayTwoDigits (hdc, st.wHour, fSuppress) ;
     else
          DisplayTwoDigits (hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress) ;

     DisplayColon (hdc) ;
     DisplayTwoDigits (hdc, st.wMinute, FALSE) ;
     DisplayColon (hdc) ;
     DisplayTwoDigits (hdc, st.wSecond, FALSE) ;
}
void DisplayTime2 (HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
     SYSTEMTIME st ;

     //GetLocalTime (&st) ;

     if (f24Hour)
          DisplayTwoDigits (hdc, st.wHour, fSuppress) ;
     else
          DisplayTwoDigits (hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress) ;

     DisplayColon (hdc) ;
     DisplayTwoDigits (hdc, st.wMinute, FALSE) ;
     DisplayColon (hdc) ;
     DisplayTwoDigits (hdc, st.wSecond, FALSE) ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static BOOL   f24Hour, fSuppress ;
     static HBRUSH hBrushRed ;
     static int    cxClient, cyClient ;
     HDC           hdc ;
     PAINTSTRUCT   ps ;
     TCHAR         szBuffer [2] ;
//DialogBox(Instanz,"IDD_DIALOG2",hwnd,DialogProc);
     switch (message)
     {
 case WM_COMMAND:
    switch(wParam){
        case IDM_MODE_WAHL:
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DialogProc);
                        //DialogBox(h(NULL), MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DialogProc);
break;
        case IDM_MODE_EXIT:
           if (MessageBox(hwnd, TEXT("Close the program?"), TEXT("Close"),
			MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			DestroyWindow(hwnd);
		}
		break;
           // SendMessage(hwnd,WM_CLOSE,0,0L);

    }


     case WM_CREATE:
          hBrushRed = CreateSolidBrush (RGB (255, 0, 0)) ;
          SetTimer (hwnd, ID_TIMER, 1000, NULL) ;

                                                  // fall through

     case WM_SETTINGCHANGE:
          GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2) ;
          f24Hour = (szBuffer[0] == '1') ;

          GetLocaleInfo (LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2) ;
          fSuppress = (szBuffer[0] == '0') ;

          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;

     case WM_SIZE:
          cxClient = LOWORD (lParam) ;
          cyClient = HIWORD (lParam) ;
          return 0 ;

     case WM_TIMER:
          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;

     //Start painiting
     case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);

			SetMapMode(hdc, MM_ISOTROPIC);
			SetWindowExtEx(hdc, 276, 72, NULL);
			SetViewportExtEx(hdc, cxClient, cyClient, NULL);

			SetWindowOrgEx(hdc, 138, 36, NULL);
			SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
			SelectObject(hdc, GetStockObject(NULL_PEN));
			SelectObject(hdc, hBrushRed);
                if (draw2) {
			DisplayTime(hdc, f24Hour, fSuppress);
                }
                 if (draw) {
			DisplayTime2(hdc, f24Hour, fSuppress);
                }
			EndPaint(hwnd, &ps);
		//}
          return 0 ;

     case WM_DESTROY:
          KillTimer (hwnd, ID_TIMER) ;
          DeleteObject (hBrushRed) ;
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
//Dialog Procedure, methode die zu Kontroll der Dialog dient
LRESULT  CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButtonFlash = NULL;
	static HWND hButtonReq = NULL;
	static HWND hEdit1 = NULL;

	HWND filedit;
	HWND filedit1;
	HWND filedit2;
int editlength;
LPTSTR  edittxt;
char txt;


	//GetWindowText(hWndCtrl, buff, 1024);
	switch
		(uMsg)
	{
		case WM_COMMAND:
		{
			//show the actual time after btn is clicked
			if (LOWORD(wParam) == IDC_RADIO1 )
			{
				if (HIWORD(wParam) == BN_CLICKED){
				 //EndDialog(hDlg, 0);
				 ShowWindow (hDlg, SW_HIDE) ;
					draw2 = true;
				}
			}
			if (LOWORD(wParam) == IDC_RADIO2 )
			{
				if (HIWORD(wParam) == BN_CLICKED){
				 //EndDialog(hDlg, 0);
				/*GetDlgItemText ( hDlg , IDC_EDIT1 , buff , 128 );
				GetDlgItemText ( hDlg , IDC_EDIT2 , buff2 , 128 );
				GetDlgItemText ( hDlg , IDC_EDIT3 , buff3  , 128 );

            filedit = GetDlgItem(hDlg, IDC_EDIT1); // I tried with and without this
            editlength = GetWindowTextLength(filedit);
            GetWindowText(filedit, buff, 1024);

            //MessageBox(hDlg, buff, "edit text", 0);

            filedit1 = GetDlgItem(hDlg, IDC_EDIT2); // I tried with and without this
            editlength = GetWindowTextLength(filedit1);
            GetWindowText(filedit1, buff2, 1024);
              //MessageBox(hDlg, buff2, "edit text", 0);

                filedit2 = GetDlgItem(hDlg, IDC_EDIT3); // I tried with and without this
            editlength = GetWindowTextLength(filedit2);
            GetWindowText(filedit2, buff3, 1024);
             // MessageBox(hDlg, buff3, "edit text", 0);*/
            GetDlgItemText(hDlg, IDC_EDIT1,buff,sizeof(buff));

                draw=true;
				// ShowWindow (hDlg, SW_HIDE) ;
					//draw2 = true;
				}
			}
		//	}
			//close the window
			if (LOWORD(wParam) == IDD_CANCEL)
			{
				if (HIWORD(wParam) == BN_CLICKED){
						if (MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
			MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			 ShowWindow (hDlg, SW_HIDE) ;
		}
				}
			}

		}
			break;



	case WM_CLOSE:
		if (MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
			MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			DestroyWindow(hDlg);
		}
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}

	return FALSE;
}
