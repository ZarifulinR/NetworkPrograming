#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include"resource.h"
#include<CommCtrl.h>
#include<cstdio>
#include <iostream>
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID SetIPPrefix(HWND hwnd);
VOID PrintInfo(HWND hwnd);
CHAR* IPaddressToString(DWORD dwIpaddress, CHAR sz_IPaddress[]);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hSpin = GetDlgItem(hwnd, IDC_SPINP_REFIX);
		SendMessage(hSpin, UDM_SETRANGE, 0, MAKELPARAM(30, 1));
		SendMessage(hSpin, UDM_SETPOS, 0, 1);
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}
	break;
	case WM_COMMAND:
	{
		HWND hIPAddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hIPprefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		DWORD dwIPaddress = 0;
		DWORD dwIPmask = 0;
		switch (LOWORD(wParam))
		{
		case IDC_IPADDRESS:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				SendMessage(hIPAddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
				if (FIRST_IPADDRESS(dwIPaddress) < 128)SendMessage(hIPmask, IPM_SETADDRESS, 0, 0xFF000000);
				else if (FIRST_IPADDRESS(dwIPaddress) < 192)SendMessage(hIPmask, IPM_SETADDRESS, 0, 0xFFFF0000);
				else if (FIRST_IPADDRESS(dwIPaddress) < 224)SendMessage(hIPmask, IPM_SETADDRESS, 0, 0xFFFFFF00);
				SetIPPrefix(hwnd);
			}
		}
		break;
		/*case IDC_IPMASK:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
				DWORD count = 0;
				for (DWORD i = dwIPmask; 0x80000000 & i; i <<= 1, count++);
				CHAR szIPprefix[3] = "";
				sprintf(szIPprefix, "%i", count);
				SendMessage(hIPprefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
			}
		}
			break;*/
		case IDC_EDIT_PREFIX:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				DWORD dwIPmask = UINT_MAX;
				CHAR szIPprefix[3];
				SendMessage(hIPprefix, WM_GETTEXT, 3, (LPARAM)szIPprefix);
				DWORD dwIPprefix = atoi(szIPprefix);
				dwIPmask <<= (32 - dwIPprefix);
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPmask);
				SetIPPrefix(hwnd);
				//PrintInfo();
			}
		}
		break;
		case IDOK:
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
		PrintInfo(hwnd);
	}
	break;
	case WM_NOTIFY:
	{
		if (wParam == IDC_IPMASK || wParam == IDC_IPADDRESS)
		{
			//	//std::cout << "WM_NOTYFY:IDC_MASK" << std::  endl;
			//	//std::cout << ((NMIPADDRESS*)lParam)->hdr.idFrom << std::endl;
			//	HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
			//	HWND hIPprefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			//	DWORD dwIPmask = 0;
			//	SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			//	DWORD count = 0;
			//	for (DWORD i = dwIPmask; 0x80000000 & i; i <<= 1, count++);
			//	CHAR szIPprefix[3] = "";
			//	sprintf(szIPprefix, "%i", count);
			//	SendMessage(hIPprefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);

			SetIPPrefix(hwnd);
		}
		PrintInfo(hwnd);
	}
	break;

	case WM_CLOSE:
		FreeConsole();
		EndDialog(hwnd, 0);
	}
	return FALSE;

}
VOID SetIPPrefix(HWND hwnd)
{

	HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
	HWND hIPprefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
	DWORD dwIPmask = 0;
	SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
	DWORD count = 0;
	for (DWORD i = dwIPmask; 0x80000000 & i; i <<= 1, count++);
	CHAR szIPprefix[3] = "";
	sprintf(szIPprefix, "%i", count);
	SendMessage(hIPprefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
}
VOID PrintInfo(HWND hwnd)
{
	CONST INT SIZE = 1024;
	CHAR sz_info[SIZE]{};
	CHAR sz_buffer[SIZE]{};
	CHAR sz_NetwordcIP_buffer[SIZE];
	CHAR sz_BroadcastIP_buffer[SIZE];
	CHAR sz_NumberOfIPs[5];
	CHAR sz_Prefix[3];
	HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
	HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
	HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
	HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
	
	
	DWORD dwIPaddress = 0;
	DWORD dwIPmask = 0;
	DWORD dwIPprefix = 0;

	SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
	SendMessage(hIPmask ,IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
	SendMessage(hEditPrefix, WM_GETTEXT, 3, (LPARAM)sz_Prefix);
	dwIPprefix = atoi(sz_Prefix);

	sprintf(sz_NetwordcIP_buffer, "јдрес сети:\t\t\t %s", IPaddressToString(dwIPaddress & dwIPmask, sz_buffer));
	sprintf(sz_BroadcastIP_buffer, "Ўироковещательный адрес: %s", IPaddressToString(dwIPaddress | ~dwIPmask, sz_buffer));
	sprintf(sz_info, "%s\n%s", sz_NetwordcIP_buffer, sz_BroadcastIP_buffer);
	SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);
}
CHAR* IPaddressToString(DWORD dwIpaddress,CHAR sz_IPaddress[])
{
	sprintf
	(
		sz_IPaddress,
		"%i.%i.%i.%i",
		FIRST_IPADDRESS(dwIpaddress),
		SECOND_IPADDRESS(dwIpaddress),
		THIRD_IPADDRESS(dwIpaddress),
		FOURTH_IPADDRESS(dwIpaddress)
	);
	return sz_IPaddress;
}