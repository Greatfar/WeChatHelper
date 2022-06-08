#include "stdafx.h"
#include "ChatRecord.h"
#include "CAutoFunction.h"
#include <stdio.h>


// ���ص�ַ��ִ����HOOK����ת�����ĵ�ַ��
DWORD RetkReciveMsgAddr = GetWeChatWinBase() + WxReciveMessage + 5;

// �����ǵ�call�ĵ�ַ
DWORD OverReciveMsgCallAddr = GetWeChatWinBase() + WxReciveMessageCall;



//************************************************************
// ��������: HookChatRecord
// ����˵��: HOOK�����¼
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
void HookChatRecord()
{
    HookAnyAddress(GetWeChatWinBase() + WxReciveMessage, RecieveWxMesage);
}



//************************************************************
// ��������: RecieveMesage
// ����˵��: ������Ϣ
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
__declspec(naked) void RecieveWxMesage()
{
    // �����ֳ�
    __asm
    {
        pushad;
        push eax;
        call SendWxMessage;  // �����Զ���ĺ���
        popad;
        // ���ñ����ǵ�call
        call OverReciveMsgCallAddr;
        // ��ת�����ص�ַ
        jmp RetkReciveMsgAddr;
    }
}



//************************************************************
// ��������: SendMessage
// ����˵��: �����յ�����Ϣ���͸��ͻ��ˣ�΢�����֣�
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
void __stdcall SendWxMessage(DWORD r_eax)
{
    try
    {
        // �������Ƿ���Ȩ�޷���ָ�����ڴ��
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

        // ȡ����Ϣ���� 3.6.0.18 δ�ҵ���Ϣ����ƫ��
        // msg->dwtype = *((DWORD*)(r_eax + MsgTypeOffset));

        // ȡ����Ϣ����
        LPVOID pContent = *((LPVOID *)(r_eax + MsgContentOffset));
        swprintf_s(msg->content, L"%s", (wchar_t*)pContent);

        // ȡ��΢��ID/ȺID
        LPVOID pWxid = *((LPVOID *)(r_eax + WxidOffset));
        swprintf_s(msg->wxid, L"%s", (wchar_t*)pWxid);
    
        // �������ں���Ϣ
        if (StrStrW(msg->wxid, L"gh_"))
        {
            return;
        }

        // ȡ����Ϣ������
        // LPVOID pSender = *((LPVOID *)(r_eax + GroupMsgSenderOffset));
        // swprintf_s(msg->sender, L"%s", (wchar_t*)pSender);

        // ȡ����Ϣ��Դ
        // LPVOID pSource = *((LPVOID *)(r_eax + MsgSourceOffset));
        // swprintf_s(msg->source, L"%s", (wchar_t*)pSource);

        // �����̴߳�����Ϣ
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DealWithMsg, msg, 0, NULL);
        hThread != 0 && CloseHandle(hThread);
    }
    catch (...)
    {
        OutputDebugStringA("������Ϣ�쳣��....");
    }
}



//************************************************************
// ��������: ReceiveMsgProc
// ����˵��: ������Ϣ���̻߳ص�����
// ��    ��: Greatfar
// ʱ    ��: 2022/04/08
// ��    ��: Context ������
// �� �� ֵ: void 
//************************************************************
void DealWithMsg(LPVOID Context)
{
    try
    {
        ChatMessageData* msg = (ChatMessageData*)Context;

        // ������Ϣ�����ƶ�
        HWND hWnd = FindWindow(NULL, TEXT("΢������"));
        if (hWnd == NULL)
        {
            OutputDebugStringA("δ���ҵ�΢�����ִ���");
            return;
        }
        COPYDATASTRUCT chatmsg;
        chatmsg.dwData = WM_ShowChatRecord;
        chatmsg.cbData = sizeof(ChatMessageData);
        chatmsg.lpData = msg;
        SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&chatmsg);

        // �ͷ���Դ
        delete msg;
    }
    catch (...)
    {
        OutputDebugStringA("�����¼���쳣��");
    }
}

