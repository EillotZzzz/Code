#include "Main.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmd, int nCmdShow)
{
	AppHInstance = hInstance;
	DialogBox(AppHInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, MainCallBack);


	return 0;
}

INT_PTR CALLBACK MainCallBack (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{


	switch (message)
	{
	case WM_INITDIALOG:
	{		

		InitLv(hwnd);		
		EnumProcess(hwnd);
	}
	return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
	}

	case WM_NOTIFY:
	{
		NMHDR* nmh = (NMHDR*)lparam;
		if (wparam == IDC_LIST_PROC &&nmh->code == NM_CLICK)
		{
			

			//ListView_GetNextItem(nmh->hwndFrom, 0, LVNI_ALL);
			int n = SendMessage(nmh->hwndFrom, LVM_GETNEXTITEM, -1, LVIS_SELECTED);
			if (n == -1)return TRUE;

			TCHAR Pid[20] = { 0 };
			ListView_GetItemText(nmh->hwndFrom, n, 1, Pid, sizeof(Pid));

			GetMoudels(hwnd, _wtoi(Pid));
			return TRUE;
		}
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wparam)
		{
		case IDC_BUTTON_PELOOK:
		{
			OPENFILENAME ofn = { 0 };
			TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
			ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
			ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
			
			ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
			ofn.nMaxFile = sizeof(strFilename);//����������
			ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
			ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
			if (GetOpenFileName(&ofn))
			{
				TCHAR *FileBuffer = NULL;
				if (!OpenFileToFileBuffer(strFilename,&FileBuffer))
				{
					return TRUE;
				}

				if (*(DWORD*)FileBuffer != 0x905a4d)
				{
					MessageBox(NULL, TEXT("����PE�ļ�"), 0, MB_OK);
					return TRUE;
				}
				m_FileBuffer = FileBuffer;
				DialogBox(AppHInstance, MAKEINTRESOURCE(IDD_DIALOG_PEFORMAT), hwnd, PECallBack);


			}
			else {
				MessageBox(NULL, TEXT("��ѡ��һ���ļ�"), NULL, MB_ICONERROR);
			}


		
		}
		}
	}

	}

	return FALSE;
}

void InitLv(HWND DlgHwnd)
{
	LVCOLUMN lv;
	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 150;
	lv.pszText = TEXT("����");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("����");

	HWND hwnd = GetDlgItem(DlgHwnd, IDC_LIST_PROC);
	ListView_InsertColumn(hwnd, 0, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 50;
	lv.pszText = TEXT("PID");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("PID");
	ListView_InsertColumn(hwnd, 1, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 100;
	lv.pszText = TEXT("�����ַ");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("�����ַ");	
	ListView_InsertColumn(hwnd, 2, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 100;
	lv.pszText = TEXT("�����С");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("�����С");
	ListView_InsertColumn(hwnd, 3, &lv);

	
	ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);

///////////////////////////////////////////////////////////////////

	hwnd = GetDlgItem(DlgHwnd, IDC_LIST_MODLUE);
	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 200;
	lv.pszText = TEXT("ģ��·��");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("ģ��·��");
	ListView_InsertColumn(hwnd, 0, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 100;
	lv.pszText = TEXT("ģ���ַ");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("ģ���ַ");
	ListView_InsertColumn(hwnd, 1, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 100;
	lv.pszText = TEXT("ģ���С");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("ģ���С");
	ListView_InsertColumn(hwnd, 2, &lv);

	//���ø���
	ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);
}

void EnumProcess(HWND DlgHwnd)
{
	int countProcess = 0;                                  //��ǰ����������������
	PROCESSENTRY32 currentProcess;                         //��ſ��ս�����Ϣ��һ���ṹ��
 	currentProcess.dwSize = sizeof(currentProcess);        //��ʹ������ṹ֮ǰ�����������Ĵ�С
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��ϵͳ�ڵ����н�����һ������

	if (hProcess == INVALID_HANDLE_VALUE)
	{		
		return;
	}

	int bMore = Process32First(hProcess, &currentProcess);    //��ȡ��һ��������Ϣ
	int n = GetLastError();
	int i = 0;
	while (bMore)
	{
		LVITEM lv;
		lv.mask = LVIF_TEXT;
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = currentProcess.szExeFile;
		SendDlgItemMessage(DlgHwnd, IDC_LIST_PROC, LVM_INSERTITEM, i, (LPARAM)&lv);

		lv.mask = LVIF_TEXT;
		lv.iItem = i;
		lv.iSubItem = 1;
		TCHAR strbuff[50] = { 0 };
		lv.pszText = _itow(currentProcess.th32ProcessID, strbuff,10);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_PROC, LVM_SETITEMTEXT, i,(LPARAM)&lv);
		


		DWORD Imagebase = 0;
		DWORD ImageSize = 0;
		GetModuleList(currentProcess.th32ProcessID, &Imagebase, &ImageSize);

		lv.mask = LVIF_TEXT;
		lv.iItem = 0;
		lv.iSubItem = 2;
		lv.pszText = _itow(Imagebase, strbuff, 16);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_PROC, LVM_SETITEMTEXT, i, (LPARAM)&lv);
		
		

		lv.mask = LVIF_TEXT;
		lv.iItem = 0;
		lv.iSubItem = 3;
		lv.pszText = _itow(ImageSize, strbuff, 16);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_PROC, LVM_SETITEMTEXT, i, (LPARAM)&lv);



		bMore = Process32Next(hProcess, &currentProcess);    //������һ��
		i++;
		countProcess++;
	}

	CloseHandle(hProcess);    //���hProcess���
	return;
}


BOOL EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);

	return true;
}

bool GetModuleList(DWORD dwPId, OUT DWORD *Imagebase, OUT DWORD *ImageSize)
{
	EnableDebugPriv();

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// 1. ����һ��ģ����صĿ��վ��
	EnableDebugPriv();
	hModuleSnap = CreateToolhelp32Snapshot( 
		TH32CS_SNAPMODULE,  // ָ�����յ�����
		dwPId);            // ָ������
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		int n = GetLastError();
		return false;
	}

	
	// 2. ͨ��ģ����վ����ȡ��һ��ģ����Ϣ
	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return false;
	}
	*Imagebase = (DWORD)me32.modBaseAddr;
	*ImageSize = me32.modBaseSize;

	// 3. ѭ����ȡģ����Ϣ
	//do {
	//	wprintf(L"ģ���ַ:%d,ģ���С��%d,ģ������:%s\n",
	//		me32.modBaseAddr, me32.modBaseSize, me32.szModule);
	//} while (Module32Next(hModuleSnap, &me32));
	// 4. �رվ�����˳�����
	CloseHandle(hModuleSnap);
	return true;
}

BOOL GetMoudels(HWND Dlghwnd, DWORD dwPId)
 {


 	HWND Hwnd = GetDlgItem(Dlghwnd, IDC_LIST_MODLUE);
	ListView_DeleteAllItems(Hwnd);

	

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// 1. ����һ��ģ����صĿ��վ��
	hModuleSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE,  // ָ�����յ�����
		dwPId);            // ָ������
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		int n = GetLastError();
		return false;
	}


	// 2. ͨ��ģ����վ����ȡ��һ��ģ����Ϣ
	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return false;
	}
	int i = 0;

	// 3. ѭ����ȡģ����Ϣ
	do {
		//me32.modBaseAddr, me32.modBaseSize, me32.szModule);

		LVITEM lv;
		lv.mask = LVIF_TEXT;
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = me32.szExePath;
		SendDlgItemMessage(Dlghwnd, IDC_LIST_MODLUE, LVM_INSERTITEM, i, (LPARAM)&lv);
		
		lv.mask = LVIF_TEXT;
		lv.iItem = i;
		lv.iSubItem = 1;
		TCHAR strbuff[50] = { 0 };
		lv.pszText = _itow((int)me32.modBaseAddr, strbuff, 16);
		SendDlgItemMessage(Dlghwnd, IDC_LIST_MODLUE, LVM_SETITEMTEXT, i, (LPARAM)&lv);

		lv.mask = LVIF_TEXT;
		lv.iItem = 0;
		lv.iSubItem = 2;
		lv.pszText = _itow(me32.modBaseSize, strbuff, 16);
		SendDlgItemMessage(Dlghwnd, IDC_LIST_MODLUE, LVM_SETITEMTEXT, i, (LPARAM)&lv);

		i++;
	} while (Module32Next(hModuleSnap, &me32));
	// 4. �رվ�����˳�����
	CloseHandle(hModuleSnap);
	return true;

}

BOOL OpenFileToFileBuffer(TCHAR*strFilename, TCHAR **FileBuffer)
{
	FILE *file = NULL;
	
	char *str = TCHAR2char(strFilename);
	file = fopen(str,"rb");
	if (!file)return FALSE;

	fseek(file, 0, SEEK_END);
	int FileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *p = (char*)malloc(FileSize);

	if (!p)
	{
		printf("�ļ���ʱ�������ڴ�ʧ��\r\n");
		fclose(file);
		return 0;
	}
	memset(p, 0, FileSize);
	fread(p, FileSize, 1, file);

	fclose(file);

	*FileBuffer = (TCHAR *)p;

	return TRUE;
}

INT_PTR CALLBACK PECallBack(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	
    switch (message)
    {
    case WM_INITDIALOG:
    {
        InitPe(hwnd);


    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwnd, 0);
    }
    return TRUE;

	case WM_COMMAND:
	{
		switch (wparam)
		{
		case IDC_BUTTON_PESECTION:
		{
			DialogBox(AppHInstance, MAKEINTRESOURCE(IDD_DIALOG_SECTION), hwnd, SecitonCallBack);
		}
		return TRUE;
		}
	}

    }

	


	return FALSE;
}

char* TCHAR2char(const TCHAR* STR)

{

	//�����ַ����ĳ���

	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);

	//����һ�����ֽڵ��ַ�������

	char* str = new char[sizeof(char) * size];

	//��STRת��str

	WideCharToMultiByte(CP_ACP, 0, STR, -1, str, size, NULL, FALSE);

	return str;

}

TCHAR* char2TCAHR(const char* str)

{

	int size = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);

	TCHAR* retStr = new TCHAR[size * sizeof(TCHAR)];

	MultiByteToWideChar(CP_ACP, 0, str, -1, retStr, size);

	return retStr;

}

bool InitPe(HWND DlgHwnd)
{
	if (!m_FileBuffer)return FALSE;

	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_FileBuffer;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((DWORD)pDos + pDos->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)((DWORD)pNt + 4 + IMAGE_SIZEOF_FILE_HEADER + pNt->FileHeader.SizeOfOptionalHeader);

	HWND hwnd = GetDlgItem(DlgHwnd, IDC_EDIT_POINT);
	TCHAR str[20] = { 0 };
	SetDlgItemText(DlgHwnd, IDC_EDIT_POINT, _itow(pNt->OptionalHeader.AddressOfEntryPoint,str,16));

	SetDlgItemText(DlgHwnd, IDC_EDIT2_BASE, _itow(pNt->OptionalHeader.ImageBase, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT1_FILEALIGN, _itow(pNt->OptionalHeader.FileAlignment, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_MAGIC, _itow(pNt->OptionalHeader.Magic, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_SIZEOFHEAD, _itow(pNt->OptionalHeader.SizeOfHeaders, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_NUM_SECTION, _itow(pNt->FileHeader.NumberOfSections, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_TIME, _itow(pNt->FileHeader.TimeDateStamp, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_CHECK, _itow(pNt->OptionalHeader.CheckSum, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_SIZEIMAGE, _itow(pNt->OptionalHeader.SizeOfImage, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_NUMOP, _itow(pNt->FileHeader.SizeOfOptionalHeader, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_RVANUM, _itow(pNt->OptionalHeader.NumberOfRvaAndSizes, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_SECTIONALIGN, _itow(pNt->OptionalHeader.SectionAlignment, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_TEXT, _itow(pSection->VirtualAddress, str, 16));	

	SetDlgItemText(DlgHwnd, IDC_EDIT_SIGN, _itow(pNt->FileHeader.Characteristics, str, 16));

	SetDlgItemText(DlgHwnd, IDC_EDIT_Subsystem, _itow(pNt->OptionalHeader.Subsystem, str, 16));

	
	for (int i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
 		if(strcmp((char*)pSection->Name,".rdata"))
		{
			pSection++;
		}
		else
		 break;
	}
	SetDlgItemText(DlgHwnd, IDC_EDIT_RDATA, _itow(pSection ->VirtualAddress, str, 16));

	return TRUE;
}

INT_PTR CALLBACK SecitonCallBack(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		InitSeciton(hwnd); 
		ReadSection(hwnd);


	}
	return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
	}
	return TRUE;

	}

	return FALSE;
}

BOOL InitSeciton(HWND DlgHwnd)
{
	
	LVCOLUMN lv = {0};
	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 50;
	lv.pszText = TEXT("�ڱ���");
	lv.iSubItem = 0;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("�ڱ���");

	HWND hwnd = GetDlgItem(DlgHwnd, IDC_LIST_SECTION);
	ListView_InsertColumn(hwnd, 0, &lv);

	 
	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 80;
	lv.pszText = TEXT("�ļ�ƫ��");
	lv.iSubItem = 1;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("�ļ�ƫ��");
	ListView_InsertColumn(hwnd, 1, &lv);


	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 80;
	lv.pszText = TEXT("�ļ���С");
	lv.iSubItem = 2;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("�ļ���С");
	ListView_InsertColumn(hwnd, 2, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 80;
	lv.pszText = TEXT("�����ַ");
	lv.iSubItem = 3;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("�����ַ");
	ListView_InsertColumn(hwnd, 3, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 80;
	lv.pszText = TEXT("�����С");
	lv.iSubItem = 4;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("�����С");
	ListView_InsertColumn(hwnd, 4, &lv);

	lv.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lv.fmt = LVCFMT_CENTER;
	lv.cx = 80;
	lv.pszText = TEXT("����");
	lv.iSubItem = 5;
	lv.iOrder = 0;
	lv.cchTextMax = sizeof("����");
	ListView_InsertColumn(hwnd, 5, &lv);

	return FALSE;
}

BOOL ReadSection(HWND DlgHwnd)
{
	if (!m_FileBuffer)return FALSE;

	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_FileBuffer;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((DWORD)pDos + pDos->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)((DWORD)pNt + 4 + IMAGE_SIZEOF_FILE_HEADER + pNt->FileHeader.SizeOfOptionalHeader);



	for (int i = 0; i < pNt->FileHeader.NumberOfSections; i++)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		wchar_t str[20] = { 0 };
		mbstowcs(str, (char*)pSection->Name, sizeof(str));
		lvi.pszText = str;
		SendDlgItemMessage(DlgHwnd, IDC_LIST_SECTION, LVM_INSERTITEM, i, (LPARAM)&lvi);
		int n = GetLastError();

		lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 1;
		wchar_t buf[20] = { 0 };
		lvi.pszText = _itow(pSection->PointerToRawData, buf,16);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_SECTION, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 2;
		wchar_t buf1[20] = { 0 };
		lvi.pszText = _itow(pSection->SizeOfRawData, buf1, 16);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_SECTION, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 3;
		wchar_t buf2[20] = { 0 };
		lvi.pszText = _itow(pSection->VirtualAddress, buf2, 16);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_SECTION, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 4;
		wchar_t buf3[20] = { 0 };
		lvi.pszText = _itow(pSection->Misc.VirtualSize, buf3, 16);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_SECTION, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 5;
		wchar_t buf4[20] = { 0 };
		lvi.pszText = _itow(pSection->Characteristics, buf4, 16);
		SendDlgItemMessage(DlgHwnd, IDC_LIST_SECTION, LVM_SETITEMTEXT, i, (LPARAM)&lvi);

		pSection++;
	}


	return false;
}