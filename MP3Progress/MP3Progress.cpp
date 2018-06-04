#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include <atlimage.h>

HRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND Button_SelectFile, Button_Progress,EditInfo;
char FilePath[100] = {NULL};
HWND MainHwnd;
void AppendWindowText(HWND hwnd, char* TextToAppend, bool ShouldEndl = false);
bool IsTag(char Tag[]);
void BeginProcess(bool ShouldCloseFile);
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wnd = { NULL };
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hIcon = LoadIcon(0, IDC_ICON);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = MainWndProc;
	wnd.lpszClassName = TEXT("Blank Win32 Window");
	RegisterClassEx(&wnd);
	RECT rc = { 0,0,600,500};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	MainHwnd =CreateWindow(wnd.lpszClassName, TEXT("MP3Progress"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right-rc.left,rc.bottom-rc.top, NULL, NULL, hInstance, NULL);
	ShowWindow(MainHwnd, nShowCmd);
	MSG msg = {NULL};
	while (msg.message!=WM_QUIT)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		GetMessage(&msg, 0, 0, 0);
	}
	return 0;
}

HRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		Button_SelectFile = CreateWindow(TEXT("BUTTON"), TEXT("ѡ���ļ�"), WS_CHILDWINDOW | WS_VISIBLE, 0, 0, 100, 50, hwnd, NULL, NULL, NULL);
		Button_Progress= CreateWindow(TEXT("BUTTON"), TEXT("����"), WS_CHILDWINDOW | WS_VISIBLE, 100, 0, 100, 50, hwnd, NULL, NULL, NULL);
		EditInfo = CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILDWINDOW | WS_VISIBLE|WS_VSCROLL| WS_EX_CLIENTEDGE | WS_EX_OVERLAPPEDWINDOW | ES_MULTILINE | ES_NOHIDESEL,0, 50, 200, 450, hwnd, NULL, NULL, NULL);
		break;
	case WM_COMMAND:
		if ((HWND)lParam== Button_SelectFile&&HIWORD(wParam) == BN_CLICKED)
		{
			WCHAR FileP[MAX_PATH] = {NULL};
			OPENFILENAME OpenFN = { NULL };
			OpenFN.lStructSize = sizeof(OPENFILENAME);
			OpenFN.hwndOwner = hwnd;
			OpenFN.lpstrFilter = TEXT("MP3�ļ�\0*.mp3\0\0");
			OpenFN.nMaxFile = MAX_PATH;
			OpenFN.lpstrFile = FileP;
			OpenFN.Flags = OFN_FILEMUSTEXIST;
			OpenFN.lpstrTitle = TEXT("ѡ�������ļ�");
			if (GetOpenFileName(&OpenFN))
			{
				setlocale(LC_ALL, "");
				wcstombs (FilePath,OpenFN.lpstrFile,MAX_PATH);
				AppendWindowText(EditInfo, FilePath,true);
				AppendWindowText(EditInfo, "*MP3·��������*", true);
			}
		} 
		else if ((HWND)lParam == Button_Progress&&HIWORD(wParam) == BN_CLICKED)
		{
			BeginProcess(true);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}
	return 0;
}

void AppendWindowText(HWND hwnd,char* TextToAppend,bool ShouldEndl)
{
	char TEXTO[65535];
	GetWindowTextA(hwnd, TEXTO, 65535);
	strcat(TEXTO, TextToAppend);
	if (ShouldEndl==true)
		strcat(TEXTO, "\r\n");
	SetWindowTextA(hwnd, TEXTO);
	SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
}

bool IsTag(char Tag[])
{
	for (int i=0;i<4;i++)
	{
		if ((Tag[i] - '0' < 0 && Tag[i] - '0'>9) || (Tag[i] - 'a' < 0 && Tag[i] - 'z'>0) || (Tag[i] - 'A' < 0 || Tag[i] - 'Z'>0))
			return false;
	}
	return true;
}

void BeginProcess(bool ShouldCloseFile)
{
	char ITOABuffer[30] = { NULL };
	char Header[4] = { NULL }; /*����Ϊ"ID3"������Ϊ��ǩ������*/
	char Ver; /*�汾��ID3V2.3 �ͼ�¼3*/
	char Revision; /*���汾�Ŵ˰汾��¼Ϊ0*/
	char Flag; /*��ű�־���ֽڣ�����汾ֻ��������λ���Ժ���ϸ��˵*/
	char Size[4]; /*��ǩ��С��������ǩͷ��10 ���ֽں����еı�ǩ֡�Ĵ�С*/
	int total_size;
	FILE *File = fopen(FilePath, "rb");
	if (!File)
	{
		AppendWindowText(EditInfo, "�ļ���ʧ��", true);
		return ;
	}
	fscanf(File, "%3c", Header);
	if (strcmp(Header, "ID3"))
	{
		MessageBox(MainHwnd, TEXT("��MP3û��ID3��ǩ"), TEXT("No ID3 Tag"), MB_OK);
		fclose(File);
		return ;
	}
	fscanf(File, "%c%c%c", &Ver, &Revision, &Flag);
	fscanf(File, "%4c", Size);
	total_size = (Size[0] & 0x7F) * 0x200000 + (Size[1] & 0x7F) * 0x400 + (Size[2] & 0x7F) * 0x80 + (Size[3] & 0x7F);
	AppendWindowText(EditInfo, "ID3V2.");
	AppendWindowText(EditInfo, itoa((int)Ver, ITOABuffer, 30), true);
	AppendWindowText(EditInfo, "Tag Total Size:");
	sprintf(ITOABuffer, "%d", total_size);
	AppendWindowText(EditInfo, ITOABuffer, true);
	char ID[5] = { NULL }; /*���ĸ��ַ���ʶһ��֡��˵�������ݣ��Ժ��г��õı�ʶ���ձ�*/
	char Flags[3]; /*��ű�־��ֻ������6 λ���Ժ���ϸ��˵*/
	int FSize;
	unsigned char* Pic;
	for (int i = 0;; /*i < total_size;i += 10*/)
	{
		fscanf(File, "%4c", ID);
		fscanf(File, "%4c", Size);
		fscanf(File, "%3c", Flags);
		if (!IsTag(ID))
			break;
		FSize = Size[0] * 0x100000000 + Size[1] * 0x10000 + Size[2] * 0x100 + Size[3];
		AppendWindowText(EditInfo, "TagID:");
		AppendWindowText(EditInfo, ID, true);
		if (FSize == 0)
			continue;
		Pic = new unsigned char[FSize - 1];
		//Pic = new  unsigned char[FSize];
		if (!fread(Pic, 1, FSize - 1, File))
			MessageBox(MainHwnd, TEXT("FileReadError"), TEXT("Error"), MB_OK);
		if (!strcmp(ID, "APIC"))																			//����ר��ͼƬ
		{
			AppendWindowText(EditInfo, "����ר������", true);
			AppendWindowText(EditInfo, "��ȡͼƬ...", true);
			HDC hdc = NULL;
			__try
			{
				IStream* stream = NULL;
				CreateStreamOnHGlobal(NULL, false, &stream);
				stream->Write(&Pic[13], FSize - 14, NULL);
				CImage* CI = new CImage();
				CI->Load(stream);
				stream->Release();
				/*FILE* AlbumPic = fopen("E:\\AlbumPic.jpg", "wb");
				if (!fwrite(&Pic[Offset],1, FSize- Offset, AlbumPic))
				MessageBox(hwnd, TEXT("FileWriteError"), TEXT("Error"), MB_OK);
				fclose(AlbumPic);
				CI->Load(TEXT("E:\\AlbumPic.jpg"));*/
				hdc = GetDC(MainHwnd);
				SetStretchBltMode(hdc, HALFTONE);
				SetBrushOrgEx(hdc, 0, 0, NULL);
				CI->StretchBlt(hdc, { 200,100,600,500 });
				ReleaseDC(MainHwnd, hdc);
				delete CI;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				MessageBox(NULL, TEXT("�쳣����Ԥ�ڣ���ֹ����"), TEXT("Exception Occurred"), MB_OK);
				ReleaseDC(MainHwnd, hdc);
				break;
			}
		}
		else if (!strcmp(ID, "TIT2") || !strcmp(ID, "TPE1") || !strcmp(ID, "TALB") || !strcmp(ID, "TCON") || !strcmp(ID, "TIT3"))												//�������������������
		{
			AppendWindowText(EditInfo, (char*)Pic, true);
		}
		delete[] Pic;
		i += FSize;
	}
	AppendWindowText(EditInfo, "���ҽ���", true);
	fclose(File);
}