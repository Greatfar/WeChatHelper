#pragma once
#include "stdafx.h"


void InitWindow(HMODULE hModule);    //��ʼ������
void RegisterWindow(HMODULE hModule);        //ע�ᴰ��
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);    //���ڻص�

