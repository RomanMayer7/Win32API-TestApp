#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
//-------------------------------------------------------------------------
        void ShowSystemTray()
        {
            HWND hwnd=FindWindow("Shell_TrayWnd", "");
            ShowWindow(hwnd, SW_SHOW);
        }

        void HideSystemTray()
        {
            HWND hwnd=FindWindow("Shell_TrayWnd", "");
            ShowWindow(hwnd, SW_HIDE);

        }
//-------------------------------------------------------------------------

const char g_szClassName[] = "myWindowClass";

//****************DIALOG PROCEDURE*******************************************************************
/*Dialog Procedure to process message for this dialog box.
  it's practicly the same as our main Window Procedure (but not exactly).*/
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM
                           lParam)
{
    switch(Message)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwnd, IDOK);
            break;
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}


//***************WINDOW PROCEDURE****************************************************************************
//Function which processing different Messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:
    {
        /*An alternative to using a menu resource is to create one on the fly (or when your program runs). This is a bit more
          work programming wise, but adds flexibility and is sometimes necessary.*/
            HMENU hMenu, hSubMenu;
            HICON hIcon, hIconSm;
            hMenu = CreateMenu();

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "&File");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_STUFF_GO, "&Go");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "&Stuff");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "&Help");

            SetMenu(hwnd, hMenu);
            //We can't use LoadIcon() at all because it will only load resources, not files
            hIcon = (HICON)LoadImage(NULL, "play.ico", IMAGE_ICON, 32, 32,
                              LR_LOADFROMFILE);
            if(hIcon)
                SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            else
                MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK |
                           MB_ICONERROR);
            hIconSm = (HICON)LoadImage(NULL, "play.ico", IMAGE_ICON, 16, 16,
                                LR_LOADFROMFILE);
            if(hIconSm)
                SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
            else
                MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK |
                           MB_ICONERROR);
    }
    break;
    case WM_COMMAND:
        /*This switch()'s
          on the value of the low word of wParam, which in the case of WM_COMMAND contains the control or menu id that
          sent the message.*/
        switch(LOWORD(wParam))
        {
          /*
            We obviously want the Exit menu item to close the program. So in the WM_COMMAND,ID_FILE_EXIT handler we
            can use the following code to do just that.*/
          case ID_FILE_EXIT:
              PostMessage(hwnd, WM_CLOSE, 0, 0);
          break;
          case ID_STUFF_GO:
          break;
          case ID_HELP_ABOUT:
            {
                int ret = DialogBox(GetModuleHandle(NULL),
                                    MAKEINTRESOURCE(IDD_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
                if(ret == IDOK)
                {
                    MessageBox(hwnd, "Dialog exited with IDOK.", "Notice",
                               MB_OK | MB_ICONINFORMATION);
                }
                else if(ret == IDCANCEL)
                {
                    MessageBox(hwnd, "Dialog exited with IDCANCEL.", "Notice",
                               MB_OK | MB_ICONINFORMATION);
                }
                else if(ret == -1)
                {
                    MessageBox(hwnd, "Dialog failed!", "Error",
                               MB_OK | MB_ICONINFORMATION);
                }
            }
            break;
// Other menu commands...

        }
    break;
    case WM_LBUTTONDOWN:
    {
        char szFileName[MAX_PATH];
        HINSTANCE hInstance = GetModuleHandle(NULL);//Get handle to current Executable
        GetModuleFileName(hInstance, szFileName, MAX_PATH);
        MessageBox(hwnd, szFileName, "This program is hiding System Tray:", MB_OK |
                   MB_ICONINFORMATION);

        //ShowWindow(hwnd, SW_HIDE);
         HideSystemTray();

    }
    break;
    case WM_RBUTTONDOWN:
        {
           MessageBox(NULL, "Right Button Clicked!Show System Tray", "Show System Tray!",
           MB_ICONEXCLAMATION | MB_OK);

           //ShowWindow(hwnd, SW_SHOW);
           ShowSystemTray();
        }
    break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}



//*****************WIN32 PROGRAMM MAIN ENTRY POINT*****************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
//--------------------------ICON---------------------------------------------------------------
    //A Function to load Icon(in this example from resource file,defined as 'IDI_MYICON')
    //Old function works only for small icons,generally 32x32 pix
    // HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));

    //Other Function to load Icon(in this example from resource file,defined as 'IDI_MYICON')
    HICON hMyIcon = static_cast<HICON>(::LoadImage(hInstance,
    MAKEINTRESOURCE(IDI_MYICON),
    IMAGE_ICON,
    48, 48,    // or whatever size icon you want to load
    LR_DEFAULTCOLOR));


    if (hMyIcon == NULL)
   {
    // Handle null-pointer error
    printf("Error Load Icon");

   }
//------------------------WINDOW STRUCTURE--------------------------------------------------------

    //Create Window Class Structure
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW ;//wc.style =  0
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = hMyIcon;//Load Icon from Resource File
    wc.hIconSm = hMyIcon; //wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION)
    wc.hCursor = LoadCursor(NULL, IDC_HAND);//wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+2);//+1 White,+2-Gray, +3-black
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);//Load Menu from Resource File
    wc.lpszClassName = g_szClassName;
    ;
//--------------------------------------------------------------------------------------------------
    //Try To Register our previously created Window Class Structure
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    //-----------------------------------------------------------------------------------------------
    //Create New Window Object based on previously registred Window Class Structure
    hwnd = CreateWindowEx(
               WS_EX_CLIENTEDGE,
               g_szClassName,
               "My Win32 API - C/C++ Program",
               WS_OVERLAPPEDWINDOW,
               CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
               NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)//check if it was actually created
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    //--------------------------------------------------------------------------------------------------
    //Show previously created  Window Object ,by passing a Handle(hwnd) to ShowWindow function
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    //-------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------
    //MESSAGE LOOP processing different Messages which sent to Window
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
