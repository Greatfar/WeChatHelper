#pragma once

// 客户端和服务端通讯消息
#define WM_Login 0
#define WM_Logout 2
#define WM_ShowChatRecord 4
#define WM_GetInformation 7
#define WM_AlreadyLogin 21
#define WM_DesktopShowWindow 45
#define WM_WechatVersionError 46


// 窗口通讯的自定义消息
#define WM_SHOWTASK WM_USER+103


// 定时器id
#define TIMER_HEALTH_CHECK 1
