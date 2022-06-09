#include "stdafx.h"
#include "CPublic.h"

using std::wstring;


//************************************************************
// ��������: HookAnyAddress
// ����˵��: Hook�����ַ
// ��    ��: Greatfar
// ʱ    ��: 2022/06/09
// ��    ��: dwHookAddr ��ҪHOOK�ĵ�ַ dwJmpAddress��ת�ĵ�ַ dwBackAddress ���صĵ�ַ
// �� �� ֵ: void 
//************************************************************
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress)
{
    // ��װ��ת����
    BYTE jmpCode[5] = { 0 };
    jmpCode[0] = 0xE9;  // jmp��Ӧ�Ļ������ʮ��������E9

    // ����jmpָ���ƫ��  ��ʽ��Ҫ��ת�ĵ�ַ - HOOK��ַ - 5
    *(DWORD*)& jmpCode[1] = (DWORD)dwJmpAddress - dwHookAddr - 5;

    // ������ǰ���������ڻ�ԭ
    DWORD OldProtext = 0;

    // ��ΪҪ�������д�����ݣ�����Ϊ������ǲ���д�ģ�������Ҫ�޸�����
    VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

    // д���Լ��Ĵ���
    memcpy((void*)dwHookAddr, jmpCode, 5);

    // ִ�����˲���֮����Ҫ���л�ԭ
    VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}



//************************************************************
// ��������: GetWeChatWinBase
// ����˵��: ��ȡWeChatWin��ַ
// ��    ��: Greatfar
// ʱ    ��: 2022/06/09
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
DWORD GetWeChatWinBase()
{
    return (DWORD)GetModuleHandle(TEXT("WeChatWin.dll"));
}



//************************************************************
// ��������: UTF8ToUnicode
// ����˵��: ��UTF8����תΪUnicode(΢��Ĭ�ϱ���ΪUTF8)
// ��    ��: Greatfar
// ʱ    ��: 2022/06/09
// ��    ��: str ��Ҫת�����ַ���
// �� �� ֵ: wchar_t ���ص��ַ��� 
//************************************************************
wchar_t * UTF8ToUnicode(const char* str)
{
    int textlen = 0;
    wchar_t * result;
    textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
    result != 0 && memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
    return result;
}



//************************************************************
// ��������: UTF8ToWstring
// ����˵��: utf-8 ת unicode (utf-16)
// ��    ��: Greatfar
// ʱ    ��: 2022/06/09
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
wstring UTF8ToWstring(const char* str)
{
    int textlen = 0;
    wchar_t* result;
    textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
    memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
    wstring tempstr(result);
    free(result);
    result = NULL;
    return tempstr;
}



//************************************************************
// ��������: DebugCode
// ����˵��: ����ָ���ַ
// ��    ��: Greatfar
// ʱ    ��: 2022/06/09
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
void DebugCode(LPVOID pAddress)
{
    char buff[40] = { 0 };
    sprintf_s(buff, "%p", pAddress);
    MessageBoxA(0, buff, "��ַ", 0);
}

