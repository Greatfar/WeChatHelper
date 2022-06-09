#pragma once

BOOL InjectDll(HANDLE& wxPid);                     // 注入dll
BOOL CheckIsInject(DWORD dwProcessid);             // 检测DLL是否已经注入

