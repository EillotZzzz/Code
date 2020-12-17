#ifndef _MAIN_
#define _MAIN_



#pragma once
#include <windows.h>
#include "resource.h"
#include <CommCtrl.h>
#include <tlhelp32.h> 
#include <stdio.h>


HINSTANCE AppHInstance;
TCHAR* m_FileBuffer;


INT_PTR CALLBACK MainCallBack(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
INT_PTR CALLBACK PECallBack(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
INT_PTR CALLBACK SecitonCallBack(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void InitLv(HWND DlgHwnd);
void EnumProcess(HWND DlgHwnd);

//提高进程权限函数
//提升当前进程权限函数("SeDebugPrivilege"读、写控制权限)
//
//返回值：“false”是失败，“true”是成功。
//
BOOL EnableDebugPriv();


//跟住PID获取进程信息
bool GetModuleList(DWORD dwPId, OUT DWORD *Imagebase, OUT DWORD *ImageSize);

BOOL GetMoudels(HWND Dlghwnd, DWORD dwPId);

BOOL OpenFileToFileBuffer(TCHAR*strFilename, TCHAR **FileBuffer);

char* TCHAR2char(const TCHAR* STR);

bool InitPe(HWND DlgHwnd);

BOOL InitSeciton(HWND DlgHwnd);

BOOL ReadSection(HWND DlgHwnd);


#endif