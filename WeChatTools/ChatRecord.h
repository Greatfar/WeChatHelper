#pragma once

void HookChatRecord();        //HOOK�����¼
void RecieveWxMesage();        //����΢����Ϣ
void __stdcall SendWxMessage(DWORD r_eax);        //����΢����Ϣ���ͻ���
void DealWithMsg(LPVOID Context);
void __stdcall SendWxMessage(DWORD r_eax);
