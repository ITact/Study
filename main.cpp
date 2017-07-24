#include <windows.h>   
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "CController.h"
#include "CTimer.h"
#include "resource.h"
#include "CParams.h"

char*			szApplicationName = "�Զ�ɨ��";
char*			szWindowClassName = "sweeper";

bool pStop = false;

//ģ�������
CController*	g_pController	 = NULL; 

//����һ��������
CParams   g_Params;

//	�˳�ʱ�������
void Cleanup()
{
	if (g_pController) 

		delete g_pController;
}

LRESULT CALLBACK WindowProc(HWND hwnd, 
						                UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	//�̶����ڳߴ�
	static int cxClient, cyClient;

	static HDC		  hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap; 


	switch(msg)
	{	
		case WM_CREATE:  //��������ʱ
		{
			//���������
			srand((unsigned) time(NULL));

			//��ȡ��ǰ���ڵĴ�С
			RECT rect;
			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;

			//���п�����
			g_pController = new CController(hwnd);

			hdcBackBuffer = CreateCompatibleDC(NULL);

			HDC hdc = GetDC(hwnd);

			hBitmap = CreateCompatibleBitmap(hdc,
											                cxClient,
											                cyClient);
			ReleaseDC(hwnd, hdc);

			hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap); 
		} 
			
		break;
		
		//�����¼�
		case WM_KEYUP:
		{
			switch(wparam)
			{

				case VK_ESCAPE:
				{
					PostQuitMessage(0);
				}

				break;

				case 'F':
					{
						g_pController->FastRenderToggle();
					}
					
					break;

				case 'R':
				  {
					 if (g_pController)
					 {
					   delete g_pController;
					 }

					 //�������п�����
					 g_pController = new CController(hwnd);
				  }	
				case 'P':
					{
						if(pStop) pStop = false;
						else pStop = true;
					}
				break;
			}
		}

		break;

		//�ı䴰���С
		case WM_SIZE:
		{
 			cxClient = LOWORD(lparam);
			cyClient = HIWORD(lparam);

			SelectObject(hdcBackBuffer, hOldBitmap);

			HDC hdc = GetDC(hwnd);

			hBitmap = CreateCompatibleBitmap(hdc,
											                 cxClient,
											                 cyClient);
			ReleaseDC(hwnd, hdc);
			
			hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap); 
		} 

		break;

		case WM_PAINT: 
		{
		PAINTSTRUCT ps;
      
		BeginPaint(hwnd, &ps);
		
		BitBlt(hdcBackBuffer,
			 0,
			 0,
			 cxClient,
			 cyClient,
			 NULL,
			 NULL,
			 NULL,
             WHITENESS);
			
			//��Ⱦ���׺�ɨ�׻�
			g_pController->Render(hdcBackBuffer);
			
			BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY); 

			EndPaint(hwnd, &ps);
		} 
			
		break;

		case WM_DESTROY: 
		{
			SelectObject(hdcBackBuffer, hOldBitmap);
			
			DeleteDC(hdcBackBuffer);
			DeleteObject(hBitmap); 

			PostQuitMessage(0);
 		} 
			
		break;

		default:break;

	}

	// Ĭ����Ϣ��� 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

}


int WINAPI WinMain(	HINSTANCE hinstance,
					          HINSTANCE hprevinstance,
					          LPSTR lpcmdline,
					          int ncmdshow)
{

	WNDCLASSEX winclass; 
	HWND	   hwnd;	 
	MSG		   msg;		 

	// ��һ��������winclass
	winclass.cbSize       = sizeof(WNDCLASSEX);
	winclass.style			  = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			  = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor		  = LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground= NULL; 
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName= szWindowClassName;
	winclass.hIconSm      = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));


	//ע�ᴰ��
	if (!RegisterClassEx(&winclass))
	{
		MessageBox(NULL, "Error Registering Class!", "Error", 0);
    return 0;
	}

	// ����һ�����ܸı�Ĵ���
	if (!(hwnd = CreateWindowEx(NULL,									
								              szWindowClassName,						
								              szApplicationName,						
								              WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                              GetSystemMetrics(SM_CXSCREEN)/2 - CParams::WindowWidth/2,
                              GetSystemMetrics(SM_CYSCREEN)/2 - CParams::WindowHeight/2,									
								              CParams::WindowWidth,
                              CParams::WindowHeight,				
								              NULL,									
								              NULL,								
								              hinstance,								
								              NULL)))	
	{
    MessageBox(NULL, "Error Creating Window!", "Error", 0);
		return 0;
	}
	
	//��ʾ����
	ShowWindow(hwnd, SW_SHOWDEFAULT );
	UpdateWindow(hwnd);

	//����һ����ʱ��
	CTimer timer(CParams::iFramesPerSecond);

	//��ʼ��ʱ��
	timer.Start();

	//������Ϣѭ��
	bool bDone = FALSE;

	while(!bDone)
	{
					
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{
			if( msg.message == WM_QUIT ) 
			{
				//�˳���Ϸ��Ϣ
				bDone = TRUE;
			} 

			else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
							
		if (pStop && (timer.ReadyForNextFrame() || g_pController->FastRender()) )
		{	
		  if(!g_pController->Update())
			{
				//���������˳���
				bDone = TRUE;
			}
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
		}									
	}

    Cleanup();
    UnregisterClass( szWindowClassName, winclass.hInstance );
	
	return 0;

}



