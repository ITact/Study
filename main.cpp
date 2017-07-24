#include <windows.h>   
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "CController.h"
#include "CTimer.h"
#include "resource.h"
#include "CParams.h"

char*			szApplicationName = "自动扫雷";
char*			szWindowClassName = "sweeper";

bool pStop = false;

//模拟控制器
CController*	g_pController	 = NULL; 

//创建一个参数类
CParams   g_Params;

//	退出时清除记忆
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
	//固定窗口尺寸
	static int cxClient, cyClient;

	static HDC		  hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap; 


	switch(msg)
	{	
		case WM_CREATE:  //创建窗口时
		{
			//产生随机数
			srand((unsigned) time(NULL));

			//获取当前窗口的大小
			RECT rect;
			GetClientRect(hwnd, &rect);

			cxClient = rect.right;
			cyClient = rect.bottom;

			//运行控制器
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
		
		//键盘事件
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

					 //重新运行控制器
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

		//改变窗体大小
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
			
			//渲染地雷和扫雷机
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

	// 默认消息句柄 
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

	// 第一步是设置winclass
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


	//注册窗体
	if (!RegisterClassEx(&winclass))
	{
		MessageBox(NULL, "Error Registering Class!", "Error", 0);
    return 0;
	}

	// 创建一个不能改变的窗口
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
	
	//显示窗口
	ShowWindow(hwnd, SW_SHOWDEFAULT );
	UpdateWindow(hwnd);

	//创建一个计时器
	CTimer timer(CParams::iFramesPerSecond);

	//开始计时器
	timer.Start();

	//进入消息循环
	bool bDone = FALSE;

	while(!bDone)
	{
					
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{
			if( msg.message == WM_QUIT ) 
			{
				//退出游戏消息
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
				//遇到问题退出来
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



