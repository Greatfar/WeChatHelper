#pragma once

void HookChatRecord();        //HOOK聊天记录
void RecieveWxMesage();        //接收微信消息
void __stdcall SendWxMessage(DWORD r_eax);        //发送微信消息到客户端
void DealWithMsg(LPVOID Context);
void __stdcall SendWxMessage(DWORD r_eax);
