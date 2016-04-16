// WlanTest.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <stdio.h>
#include <tchar.h>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wlanapi.h>

#define WLAN_CLIENT_VERSION_VISTA 2

void SetInterface(WLAN_INTF_OPCODE opcode, PVOID* pData, GUID* InterfaceGuid)
{
	DWORD dwResult = 0;
	HANDLE hClient = NULL;
	DWORD dwCurVersion = 0;
	DWORD outsize = 0;

	// Open Handle for the set operation
	dwResult = WlanOpenHandle(WLAN_CLIENT_VERSION_VISTA, NULL, &dwCurVersion, &hClient);
	dwResult = WlanSetInterface(hClient, InterfaceGuid, opcode, sizeof(ULONG), pData, NULL);
	WlanCloseHandle(hClient, NULL);

}

// enumerate wireless interfaces
UINT EnumInterface(HANDLE hClient, WLAN_INTERFACE_INFO sInfo[64])
{
	DWORD dwError = ERROR_SUCCESS;
	PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;
	UINT i = 0;

	__try
	{
		// enumerate wireless interfaces
		if ((dwError = WlanEnumInterfaces(
			hClient,
			NULL,               // reserved
			&pIntfList
			)) != ERROR_SUCCESS)
		{
			__leave;
		}

		// print out interface information
		for (i = 0; i < pIntfList->dwNumberOfItems; i++)
		{
			memcpy(&sInfo[i], &pIntfList->InterfaceInfo[i], sizeof(WLAN_INTERFACE_INFO));
		}

		return pIntfList->dwNumberOfItems;
	}
	__finally
	{
		// clean up
		if (pIntfList != NULL)
		{
			WlanFreeMemory(pIntfList);
		}
	}
	return 0;
}

// open a WLAN client handle and check version
DWORD
OpenHandleAndCheckVersion(
PHANDLE phClient
)
{
	DWORD dwError = ERROR_SUCCESS;
	DWORD dwServiceVersion;
	HANDLE hClient = NULL;

	__try
	{
		*phClient = NULL;

		// open a handle to the service
		if ((dwError = WlanOpenHandle(
			WLAN_API_VERSION,
			NULL,               // reserved
			&dwServiceVersion,
			&hClient
			)) != ERROR_SUCCESS)
		{
			__leave;
		}

		// check service version
		if (WLAN_API_VERSION_MAJOR(dwServiceVersion) < WLAN_API_VERSION_MAJOR(WLAN_API_VERSION_2_0))
		{
			// No-op, because the version check is for demonstration purpose only.
			// You can add your own logic here.
		}

		*phClient = hClient;

		// set hClient to NULL so it will not be closed
		hClient = NULL;
	}
	__finally
	{
		if (hClient != NULL)
		{
			// clean up
			WlanCloseHandle(
				hClient,
				NULL            // reserved
				);
		}
	}

	return dwError;
}

// get interface state string
LPWSTR
GetInterfaceStateString(__in WLAN_INTERFACE_STATE wlanInterfaceState)
{
	LPWSTR strRetCode;

	switch (wlanInterfaceState)
	{
	case wlan_interface_state_not_ready:
		strRetCode = L"\"not ready\"";
		break;
	case wlan_interface_state_connected:
		strRetCode = L"\"connected\"";
		break;
	case wlan_interface_state_ad_hoc_network_formed:
		strRetCode = L"\"ad hoc network formed\"";
		break;
	case wlan_interface_state_disconnecting:
		strRetCode = L"\"disconnecting\"";
		break;
	case wlan_interface_state_disconnected:
		strRetCode = L"\"disconnected\"";
		break;
	case wlan_interface_state_associating:
		strRetCode = L"\"associating\"";
		break;
	case wlan_interface_state_discovering:
		strRetCode = L"\"discovering\"";
		break;
	case wlan_interface_state_authenticating:
		strRetCode = L"\"authenticating\"";
		break;
	default:
		strRetCode = L"\"invalid interface state\"";
	}

	return strRetCode;
}

int main()
{
	HANDLE hClient = NULL;
	WLAN_INTERFACE_INFO sInfo[64];
	RPC_CSTR strGuid = NULL;

	TCHAR szBuffer[256];
	DWORD dwRead;
	if (OpenHandleAndCheckVersion(&hClient) != ERROR_SUCCESS)
		return -1;

	UINT nCount = EnumInterface(hClient, sInfo);
	for (UINT i = 0; i < nCount; ++i)
	{
		if (UuidToStringA(&sInfo[i].InterfaceGuid, &strGuid) == RPC_S_OK)
		{
			printf(("%d. %s\n\tDescription: %S\n\tState: %S\n"),
				i,
				strGuid,
				sInfo[i].strInterfaceDescription,
				GetInterfaceStateString(sInfo[i].isState));

			RpcStringFreeA(&strGuid);
		}
	}

	UINT nChoice = 0;
	printf("Enter the choice (0, 1,..) of the wireless card you want to operate on:\n");

	if (ReadConsole(GetStdHandle(STD_INPUT_HANDLE), szBuffer, _countof(szBuffer), &dwRead, NULL) == FALSE)
	{
		puts("Error input.");
		return -1;
	}
	szBuffer[dwRead] = 0;
	nChoice = _ttoi(szBuffer);

	if (nChoice > nCount)
	{
		puts("No such index.");
		return -1;
	}

	UINT nSTate = 0;
	//ULONG targetOperationMode = DOT11_OPERATION_MODE_EXTENSIBLE_STATION;
	ULONG targetOperationMode = DOT11_OPERATION_MODE_NETWORK_MONITOR;
	printf("Enter the state (0, 1 or 2) you want to switch to for the chosen wireless card:\n");
	printf("0: Managed Mode (ExtSTA)\n1: Monitor Mode (NetMon)\n2: Master Mode (ExtAP)\n");

	if (ReadConsole(GetStdHandle(STD_INPUT_HANDLE), szBuffer, _countof(szBuffer), &dwRead, NULL) == FALSE)
	{
		puts("Error input.");
		return -1;
	}
	szBuffer[dwRead] = 0;
	nSTate = _ttoi(szBuffer);

	if (nSTate != 0 && nSTate != 1 && nSTate != 2)
	{
		puts("Only 0, 1 and 2 are valid inputs.");
		return -1;
	}
	if (nSTate == 0)
	{
		targetOperationMode = DOT11_OPERATION_MODE_EXTENSIBLE_STATION;
	}
	if (nSTate == 1)
	{
		targetOperationMode = DOT11_OPERATION_MODE_NETWORK_MONITOR;
	}
	else // nSTate == 2
	{
		targetOperationMode = DOT11_OPERATION_MODE_EXTENSIBLE_AP;
	}

	SetInterface(wlan_intf_opcode_current_operation_mode, (PVOID*)&targetOperationMode, &sInfo[nChoice].InterfaceGuid);

	return 0;
}

