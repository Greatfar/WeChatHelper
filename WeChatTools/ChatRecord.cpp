#include "stdafx.h"
#include "ChatRecord.h"
#include "CAutoFunction.h"
#include <stdio.h>


// 返回地址（执行完HOOK后跳转回来的地址）
DWORD RetkReciveMsgAddr = GetWeChatWinBase() + WxReciveMessage + 5;

// 被覆盖的call的地址
DWORD OverReciveMsgCallAddr = GetWeChatWinBase() + WxReciveMessageCall;



//************************************************************
// 函数名称: HookChatRecord
// 函数说明: HOOK聊天记录
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: void
// 返 回 值: void 
//************************************************************
void HookChatRecord()
{
    HookAnyAddress(GetWeChatWinBase() + WxReciveMessage, RecieveWxMesage);
}



//************************************************************
// 函数名称: RecieveMesage
// 函数说明: 接收消息
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: void
// 返 回 值: void 
//************************************************************
__declspec(naked) void RecieveWxMesage()
{
    // 保存现场
    __asm
    {
        pushad;
        push eax;
        call SendWxMessage;  // 调用自定义的函数
        popad;
        // 调用被覆盖的call
        call OverReciveMsgCallAddr;
        // 跳转到返回地址
        jmp RetkReciveMsgAddr;
    }
}



//************************************************************
// 函数名称: SendMessage
// 函数说明: 将接收到的消息发送给客户端（微信助手）
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: void
// 返 回 值: void 
//************************************************************
void __stdcall SendWxMessage(DWORD r_eax)
{
    try
    {
        // 检查进程是否有权限访问指定的内存块
        if (IsBadReadPtr((void*)r_eax, 4)
            || IsBadReadPtr((void*)(r_eax + MsgTypeOffset), 4)
            || IsBadReadPtr((void*)(r_eax + MsgContentOffset), 4)
            || IsBadReadPtr((void*)(r_eax + WxidOffset), 4)
            || IsBadReadPtr((void*)(r_eax + GroupMsgSenderOffset), 4)
            || IsBadReadPtr((void*)(r_eax + MsgSourceOffset), 4)
        ){
            return;
        }

        ChatMessageData* msg = new ChatMessageData;

        // 取出消息类型 3.6.0.18 未找到消息类型偏移
        // msg->dwtype = *((DWORD*)(r_eax + MsgTypeOffset));

        // 取出消息内容
        LPVOID pContent = *((LPVOID *)(r_eax + MsgContentOffset));
        swprintf_s(msg->content, L"%s", (wchar_t*)pContent);

        // 取出微信ID/群ID
        LPVOID pWxid = *((LPVOID *)(r_eax + WxidOffset));
        swprintf_s(msg->wxid, L"%s", (wchar_t*)pWxid);
    
        // 不处理公众号消息
        if (StrStrW(msg->wxid, L"gh_"))
        {
            return;
        }

        // 取出消息发送者
        // LPVOID pSender = *((LPVOID *)(r_eax + GroupMsgSenderOffset));
        // swprintf_s(msg->sender, L"%s", (wchar_t*)pSender);

        // 取出消息来源
        // LPVOID pSource = *((LPVOID *)(r_eax + MsgSourceOffset));
        // swprintf_s(msg->source, L"%s", (wchar_t*)pSource);

        // 创建线程处理消息
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DealWithMsg, msg, 0, NULL);
        hThread != 0 && CloseHandle(hThread);
    }
    catch (...)
    {
        OutputDebugStringA("接收消息异常了....");
    }
}



//************************************************************
// 函数名称: ReceiveMsgProc
// 函数说明: 接收消息的线程回调函数
// 作    者: Greatfar
// 时    间: 2022/04/08
// 参    数: Context 上下文
// 返 回 值: void 
//************************************************************
void DealWithMsg(LPVOID Context)
{
    try
    {
        ChatMessageData* msg = (ChatMessageData*)Context;

        // 发送消息到控制端
        HWND hWnd = FindWindow(NULL, TEXT("微信助手"));
        if (hWnd == NULL)
        {
            OutputDebugStringA("未查找到微信助手窗口");
            return;
        }
        COPYDATASTRUCT chatmsg;
        chatmsg.dwData = WM_ShowChatRecord;
        chatmsg.cbData = sizeof(ChatMessageData);
        chatmsg.lpData = msg;
        SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&chatmsg);

        // 释放资源
        delete msg;
    }
    catch (...)
    {
        OutputDebugStringA("聊天记录出异常了");
    }
}

