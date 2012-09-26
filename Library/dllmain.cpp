// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

typedef int (WINAPI* connect_t)(SOCKET, const struct sockaddr *, int);
typedef int (WINAPI* WSAConnect_t)(SOCKET, const struct sockaddr *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS); 

HMODULE winsock = NULL;
connect_t connect_winsock = NULL;
WSAConnect_t WSAConnect_winsock = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(winsock == NULL) {
		HMODULE winsock = LoadLibrary(L"ws2_32_org.dll");
		connect_winsock = (connect_t) GetProcAddress(winsock, "connect");
		WSAConnect_winsock = (WSAConnect_t) GetProcAddress(winsock, "WSAConnect");
	}
	return TRUE;
}

void proxy(struct sockaddr *name) {
	HKEY hKey;
	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Klatt\\Forced Proxy", 0, NULL, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hKey, NULL) != ERROR_SUCCESS) {
		return;
	}
	TCHAR path[_MAX_PATH], fname[_MAX_FNAME];
	if(!GetModuleFileName(NULL, path, sizeof(path))) {
		return;
	}
	if(_wsplitpath_s(path, NULL, 0, NULL, 0, fname, _MAX_FNAME, NULL, 0)) {
		return;
	}
	DWORD intercept;
    DWORD cbData = sizeof(intercept);
	if(RegQueryValueEx(hKey, fname, NULL, NULL, (LPBYTE) &intercept, &cbData) != ERROR_SUCCESS) {
		intercept = 0;
		RegSetValueEx(hKey, fname, 0, REG_DWORD, (LPBYTE) &intercept, cbData);
		return;
	}
	if(intercept && name->sa_family == AF_INET) {
		char proxyName[16] ;
		cbData = sizeof(proxyName);
		if(RegQueryValueExA(hKey, NULL, NULL, NULL, (LPBYTE) &proxyName, &cbData) != ERROR_SUCCESS) {
			strcpy_s(proxyName, "127.0.0.1");
			RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE) &proxyName, cbData);
		}
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)name;
		if(ntohs(ipv4->sin_port) == 80) {
			ipv4->sin_addr.s_addr = inet_addr(proxyName);
			ipv4->sin_port = htons(8080);
		} else if(ntohs(ipv4->sin_port) == 443) {
			ipv4->sin_addr.s_addr = inet_addr(proxyName);
			ipv4->sin_port = htons(8443);
		}
	}
}

int WINAPI proxyconnect(SOCKET s, struct sockaddr *name, int namelen)
{
	proxy(name);
	return connect_winsock(s, name, namelen);
}


int WINAPI proxyWSAConnect(SOCKET s, struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS)
{
	proxy(name);
	return WSAConnect_winsock(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
}

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(linker, "/export:accept=ws2_32_org.accept,@1")
#pragma comment(linker, "/export:bind=ws2_32_org.bind,@2")
#pragma comment(linker, "/export:closesocket=ws2_32_org.closesocket,@3")
#pragma comment(linker, "/export:getpeername=ws2_32_org.getpeername,@5")
#pragma comment(linker, "/export:getsockname=ws2_32_org.getsockname,@6")
#pragma comment(linker, "/export:getsockopt=ws2_32_org.getsockopt,@7")
#pragma comment(linker, "/export:htonl=ws2_32_org.htonl,@8")
#pragma comment(linker, "/export:htons=ws2_32_org.htons,@9")
#pragma comment(linker, "/export:ioctlsocket=ws2_32_org.ioctlsocket,@10")
#pragma comment(linker, "/export:inet_addr=ws2_32_org.inet_addr,@11")
#pragma comment(linker, "/export:inet_ntoa=ws2_32_org.inet_ntoa,@12")
#pragma comment(linker, "/export:listen=ws2_32_org.listen,@13")
#pragma comment(linker, "/export:ntohl=ws2_32_org.ntohl,@14")
#pragma comment(linker, "/export:ntohs=ws2_32_org.ntohs,@15")
#pragma comment(linker, "/export:recv=ws2_32_org.recv,@16")
#pragma comment(linker, "/export:recvfrom=ws2_32_org.recvfrom,@17")
#pragma comment(linker, "/export:select=ws2_32_org.select,@18")
#pragma comment(linker, "/export:send=ws2_32_org.send,@19")
#pragma comment(linker, "/export:sendto=ws2_32_org.sendto,@20")
#pragma comment(linker, "/export:setsockopt=ws2_32_org.setsockopt,@21")
#pragma comment(linker, "/export:shutdown=ws2_32_org.shutdown,@22")
#pragma comment(linker, "/export:socket=ws2_32_org.socket,@23")
#pragma comment(linker, "/export:WSApSetPostRoutine=ws2_32_org.WSApSetPostRoutine,@24")
#pragma comment(linker, "/export:FreeAddrInfoEx=ws2_32_org.FreeAddrInfoEx,@25")
#pragma comment(linker, "/export:FreeAddrInfoExW=ws2_32_org.FreeAddrInfoExW,@26")
#pragma comment(linker, "/export:FreeAddrInfoW=ws2_32_org.FreeAddrInfoW,@27")
#pragma comment(linker, "/export:GetAddrInfoExA=ws2_32_org.GetAddrInfoExA,@28")
#pragma comment(linker, "/export:GetAddrInfoExW=ws2_32_org.GetAddrInfoExW,@29")
#pragma comment(linker, "/export:GetAddrInfoW=ws2_32_org.GetAddrInfoW,@30")
#pragma comment(linker, "/export:GetNameInfoW=ws2_32_org.GetNameInfoW,@31")
#pragma comment(linker, "/export:InetNtopW=ws2_32_org.InetNtopW,@32")
#pragma comment(linker, "/export:InetPtonW=ws2_32_org.InetPtonW,@33")
#pragma comment(linker, "/export:SetAddrInfoExA=ws2_32_org.SetAddrInfoExA,@34")
#pragma comment(linker, "/export:SetAddrInfoExW=ws2_32_org.SetAddrInfoExW,@35")
#pragma comment(linker, "/export:WPUCompleteOverlappedRequest=ws2_32_org.WPUCompleteOverlappedRequest,@36")
#pragma comment(linker, "/export:WSAAccept=ws2_32_org.WSAAccept,@37")
#pragma comment(linker, "/export:WSAAddressToStringA=ws2_32_org.WSAAddressToStringA,@38")
#pragma comment(linker, "/export:WSAAddressToStringW=ws2_32_org.WSAAddressToStringW,@39")
#pragma comment(linker, "/export:WSAAdvertiseProvider=ws2_32_org.WSAAdvertiseProvider,@40")
#pragma comment(linker, "/export:WSACloseEvent=ws2_32_org.WSACloseEvent,@41")
#pragma comment(linker, "/export:WSAConnectByList=ws2_32_org.WSAConnectByList,@43")
#pragma comment(linker, "/export:WSAConnectByNameA=ws2_32_org.WSAConnectByNameA,@44")
#pragma comment(linker, "/export:WSAConnectByNameW=ws2_32_org.WSAConnectByNameW,@45")
#pragma comment(linker, "/export:WSACreateEvent=ws2_32_org.WSACreateEvent,@46")
#pragma comment(linker, "/export:WSADuplicateSocketA=ws2_32_org.WSADuplicateSocketA,@47")
#pragma comment(linker, "/export:WSADuplicateSocketW=ws2_32_org.WSADuplicateSocketW,@48")
#pragma comment(linker, "/export:WSAEnumNameSpaceProvidersA=ws2_32_org.WSAEnumNameSpaceProvidersA,@49")
#pragma comment(linker, "/export:WSAEnumNameSpaceProvidersExA=ws2_32_org.WSAEnumNameSpaceProvidersExA,@50")
#pragma comment(linker, "/export:gethostbyaddr=ws2_32_org.gethostbyaddr,@51")
#pragma comment(linker, "/export:gethostbyname=ws2_32_org.gethostbyname,@52")
#pragma comment(linker, "/export:getprotobyname=ws2_32_org.getprotobyname,@53")
#pragma comment(linker, "/export:getprotobynumber=ws2_32_org.getprotobynumber,@54")
#pragma comment(linker, "/export:getservbyname=ws2_32_org.getservbyname,@55")
#pragma comment(linker, "/export:getservbyport=ws2_32_org.getservbyport,@56")
#pragma comment(linker, "/export:gethostname=ws2_32_org.gethostname,@57")
#pragma comment(linker, "/export:WSAEnumNameSpaceProvidersExW=ws2_32_org.WSAEnumNameSpaceProvidersExW,@58")
#pragma comment(linker, "/export:WSAEnumNameSpaceProvidersW=ws2_32_org.WSAEnumNameSpaceProvidersW,@59")
#pragma comment(linker, "/export:WSAEnumNetworkEvents=ws2_32_org.WSAEnumNetworkEvents,@60")
#pragma comment(linker, "/export:WSAEnumProtocolsA=ws2_32_org.WSAEnumProtocolsA,@61")
#pragma comment(linker, "/export:WSAEnumProtocolsW=ws2_32_org.WSAEnumProtocolsW,@62")
#pragma comment(linker, "/export:WSAEventSelect=ws2_32_org.WSAEventSelect,@63")
#pragma comment(linker, "/export:WSAGetOverlappedResult=ws2_32_org.WSAGetOverlappedResult,@64")
#pragma comment(linker, "/export:WSAGetQOSByName=ws2_32_org.WSAGetQOSByName,@65")
#pragma comment(linker, "/export:WSAGetServiceClassInfoA=ws2_32_org.WSAGetServiceClassInfoA,@66")
#pragma comment(linker, "/export:WSAGetServiceClassInfoW=ws2_32_org.WSAGetServiceClassInfoW,@67")
#pragma comment(linker, "/export:WSAGetServiceClassNameByClassIdA=ws2_32_org.WSAGetServiceClassNameByClassIdA,@68")
#pragma comment(linker, "/export:WSAGetServiceClassNameByClassIdW=ws2_32_org.WSAGetServiceClassNameByClassIdW,@69")
#pragma comment(linker, "/export:WSAHtonl=ws2_32_org.WSAHtonl,@70")
#pragma comment(linker, "/export:WSAHtons=ws2_32_org.WSAHtons,@71")
#pragma comment(linker, "/export:WSAInstallServiceClassA=ws2_32_org.WSAInstallServiceClassA,@72")
#pragma comment(linker, "/export:WSAInstallServiceClassW=ws2_32_org.WSAInstallServiceClassW,@73")
#pragma comment(linker, "/export:WSAIoctl=ws2_32_org.WSAIoctl,@74")
#pragma comment(linker, "/export:WSAJoinLeaf=ws2_32_org.WSAJoinLeaf,@75")
#pragma comment(linker, "/export:WSALookupServiceBeginA=ws2_32_org.WSALookupServiceBeginA,@76")
#pragma comment(linker, "/export:WSALookupServiceBeginW=ws2_32_org.WSALookupServiceBeginW,@77")
#pragma comment(linker, "/export:WSALookupServiceEnd=ws2_32_org.WSALookupServiceEnd,@78")
#pragma comment(linker, "/export:WSALookupServiceNextA=ws2_32_org.WSALookupServiceNextA,@79")
#pragma comment(linker, "/export:WSALookupServiceNextW=ws2_32_org.WSALookupServiceNextW,@80")
#pragma comment(linker, "/export:WSANSPIoctl=ws2_32_org.WSANSPIoctl,@81")
#pragma comment(linker, "/export:WSANtohl=ws2_32_org.WSANtohl,@82")
#pragma comment(linker, "/export:WSANtohs=ws2_32_org.WSANtohs,@83")
#pragma comment(linker, "/export:WSAPoll=ws2_32_org.WSAPoll,@84")
#pragma comment(linker, "/export:WSAProviderCompleteAsyncCall=ws2_32_org.WSAProviderCompleteAsyncCall,@85")
#pragma comment(linker, "/export:WSAProviderConfigChange=ws2_32_org.WSAProviderConfigChange,@86")
#pragma comment(linker, "/export:WSARecv=ws2_32_org.WSARecv,@87")
#pragma comment(linker, "/export:WSARecvDisconnect=ws2_32_org.WSARecvDisconnect,@88")
#pragma comment(linker, "/export:WSARecvFrom=ws2_32_org.WSARecvFrom,@89")
#pragma comment(linker, "/export:WSARemoveServiceClass=ws2_32_org.WSARemoveServiceClass,@90")
#pragma comment(linker, "/export:WSAResetEvent=ws2_32_org.WSAResetEvent,@91")
#pragma comment(linker, "/export:WSASend=ws2_32_org.WSASend,@92")
#pragma comment(linker, "/export:WSASendDisconnect=ws2_32_org.WSASendDisconnect,@93")
#pragma comment(linker, "/export:WSASendMsg=ws2_32_org.WSASendMsg,@94")
#pragma comment(linker, "/export:WSASendTo=ws2_32_org.WSASendTo,@95")
#pragma comment(linker, "/export:WSASetEvent=ws2_32_org.WSASetEvent,@96")
#pragma comment(linker, "/export:WSASetServiceA=ws2_32_org.WSASetServiceA,@97")
#pragma comment(linker, "/export:WSASetServiceW=ws2_32_org.WSASetServiceW,@98")
#pragma comment(linker, "/export:WSASocketA=ws2_32_org.WSASocketA,@99")
#pragma comment(linker, "/export:WSASocketW=ws2_32_org.WSASocketW,@100")
#pragma comment(linker, "/export:WSAAsyncSelect=ws2_32_org.WSAAsyncSelect,@101")
#pragma comment(linker, "/export:WSAAsyncGetHostByAddr=ws2_32_org.WSAAsyncGetHostByAddr,@102")
#pragma comment(linker, "/export:WSAAsyncGetHostByName=ws2_32_org.WSAAsyncGetHostByName,@103")
#pragma comment(linker, "/export:WSAAsyncGetProtoByNumber=ws2_32_org.WSAAsyncGetProtoByNumber,@104")
#pragma comment(linker, "/export:WSAAsyncGetProtoByName=ws2_32_org.WSAAsyncGetProtoByName,@105")
#pragma comment(linker, "/export:WSAAsyncGetServByPort=ws2_32_org.WSAAsyncGetServByPort,@106")
#pragma comment(linker, "/export:WSAAsyncGetServByName=ws2_32_org.WSAAsyncGetServByName,@107")
#pragma comment(linker, "/export:WSACancelAsyncRequest=ws2_32_org.WSACancelAsyncRequest,@108")
#pragma comment(linker, "/export:WSASetBlockingHook=ws2_32_org.WSASetBlockingHook,@109")
#pragma comment(linker, "/export:WSAUnhookBlockingHook=ws2_32_org.WSAUnhookBlockingHook,@110")
#pragma comment(linker, "/export:WSAGetLastError=ws2_32_org.WSAGetLastError,@111")
#pragma comment(linker, "/export:WSASetLastError=ws2_32_org.WSASetLastError,@112")
#pragma comment(linker, "/export:WSACancelBlockingCall=ws2_32_org.WSACancelBlockingCall,@113")
#pragma comment(linker, "/export:WSAIsBlocking=ws2_32_org.WSAIsBlocking,@114")
#pragma comment(linker, "/export:WSAStartup=ws2_32_org.WSAStartup,@115")
#pragma comment(linker, "/export:WSACleanup=ws2_32_org.WSACleanup,@116")
#pragma comment(linker, "/export:WSAStringToAddressA=ws2_32_org.WSAStringToAddressA,@117")
#pragma comment(linker, "/export:WSAStringToAddressW=ws2_32_org.WSAStringToAddressW,@118")
#pragma comment(linker, "/export:WSAUnadvertiseProvider=ws2_32_org.WSAUnadvertiseProvider,@119")
#pragma comment(linker, "/export:WSAWaitForMultipleEvents=ws2_32_org.WSAWaitForMultipleEvents,@120")
#pragma comment(linker, "/export:WSCDeinstallProvider=ws2_32_org.WSCDeinstallProvider,@121")
#ifdef _WIN64
#pragma comment(linker, "/export:WSCDeinstallProvider32=ws2_32_org.WSCDeinstallProvider32,@122")
#pragma comment(linker, "/export:WSCEnableNSProvider=ws2_32_org.WSCEnableNSProvider,@123")
#pragma comment(linker, "/export:WSCEnableNSProvider32=ws2_32_org.WSCEnableNSProvider32,@124")
#pragma comment(linker, "/export:WSCEnumNameSpaceProviders32=ws2_32_org.WSCEnumNameSpaceProviders32,@125")
#pragma comment(linker, "/export:WSCEnumNameSpaceProvidersEx32=ws2_32_org.WSCEnumNameSpaceProvidersEx32,@126")
#pragma comment(linker, "/export:WSCEnumProtocols=ws2_32_org.WSCEnumProtocols,@127")
#pragma comment(linker, "/export:WSCEnumProtocols32=ws2_32_org.WSCEnumProtocols32,@128")
#pragma comment(linker, "/export:WSCGetApplicationCategory=ws2_32_org.WSCGetApplicationCategory,@129")
#pragma comment(linker, "/export:WSCGetProviderInfo=ws2_32_org.WSCGetProviderInfo,@130")
#pragma comment(linker, "/export:WSCGetProviderInfo32=ws2_32_org.WSCGetProviderInfo32,@131")
#pragma comment(linker, "/export:WSCGetProviderPath=ws2_32_org.WSCGetProviderPath,@132")
#pragma comment(linker, "/export:WSCGetProviderPath32=ws2_32_org.WSCGetProviderPath32,@133")
#pragma comment(linker, "/export:WSCInstallNameSpace=ws2_32_org.WSCInstallNameSpace,@134")
#pragma comment(linker, "/export:WSCInstallNameSpace32=ws2_32_org.WSCInstallNameSpace32,@135")
#pragma comment(linker, "/export:WSCInstallNameSpaceEx=ws2_32_org.WSCInstallNameSpaceEx,@136")
#pragma comment(linker, "/export:WSCInstallNameSpaceEx32=ws2_32_org.WSCInstallNameSpaceEx32,@137")
#pragma comment(linker, "/export:WSCInstallProvider=ws2_32_org.WSCInstallProvider,@138")
#pragma comment(linker, "/export:WSCInstallProvider64_32=ws2_32_org.WSCInstallProvider64_32,@139")
#pragma comment(linker, "/export:WSCInstallProviderAndChains64_32=ws2_32_org.WSCInstallProviderAndChains64_32,@140")
#pragma comment(linker, "/export:WSCSetApplicationCategory=ws2_32_org.WSCSetApplicationCategory,@141")
#pragma comment(linker, "/export:WSCSetProviderInfo=ws2_32_org.WSCSetProviderInfo,@142")
#pragma comment(linker, "/export:WSCSetProviderInfo32=ws2_32_org.WSCSetProviderInfo32,@143")
#pragma comment(linker, "/export:WSCUnInstallNameSpace=ws2_32_org.WSCUnInstallNameSpace,@144")
#pragma comment(linker, "/export:WSCUnInstallNameSpace32=ws2_32_org.WSCUnInstallNameSpace32,@145")
#pragma comment(linker, "/export:WSCUpdateProvider=ws2_32_org.WSCUpdateProvider,@146")
#pragma comment(linker, "/export:WSCUpdateProvider32=ws2_32_org.WSCUpdateProvider32,@147")
#pragma comment(linker, "/export:WSCWriteNameSpaceOrder=ws2_32_org.WSCWriteNameSpaceOrder,@148")
#pragma comment(linker, "/export:WSCWriteNameSpaceOrder32=ws2_32_org.WSCWriteNameSpaceOrder32,@149")
#pragma comment(linker, "/export:WSCWriteProviderOrder=ws2_32_org.WSCWriteProviderOrder,@150")
#else
#pragma comment(linker, "/export:WSCEnableNSProvider=ws2_32_org.WSCEnableNSProvider,@122")
#pragma comment(linker, "/export:WSCEnumProtocols=ws2_32_org.WSCEnumProtocols,@123")
#pragma comment(linker, "/export:WSCGetApplicationCategory=ws2_32_org.WSCGetApplicationCategory,@124")
#pragma comment(linker, "/export:WSCGetProviderInfo=ws2_32_org.WSCGetProviderInfo,@125")
#pragma comment(linker, "/export:WSCGetProviderPath=ws2_32_org.WSCGetProviderPath,@126")
#pragma comment(linker, "/export:WSCInstallNameSpace=ws2_32_org.WSCInstallNameSpace,@127")
#pragma comment(linker, "/export:WSCInstallNameSpaceEx=ws2_32_org.WSCInstallNameSpaceEx,@128")
#pragma comment(linker, "/export:WSCInstallProvider=ws2_32_org.WSCInstallProvider,@129")
#pragma comment(linker, "/export:WSCInstallProviderAndChains=ws2_32_org.WSCInstallProviderAndChains,@130")
#pragma comment(linker, "/export:WSCSetApplicationCategory=ws2_32_org.WSCSetApplicationCategory,@131")
#pragma comment(linker, "/export:WSCSetProviderInfo=ws2_32_org.WSCSetProviderInfo,@132")
#pragma comment(linker, "/export:WSCUnInstallNameSpace=ws2_32_org.WSCUnInstallNameSpace,@133")
#pragma comment(linker, "/export:WSCUpdateProvider=ws2_32_org.WSCUpdateProvider,@134")
#pragma comment(linker, "/export:WSCWriteNameSpaceOrder=ws2_32_org.WSCWriteNameSpaceOrder,@135")
#pragma comment(linker, "/export:WSCWriteProviderOrder=ws2_32_org.WSCWriteProviderOrder,@136")
#pragma comment(linker, "/export:WahCloseApcHelper=ws2_32_org.WahCloseApcHelper,@137")
#pragma comment(linker, "/export:WahCloseHandleHelper=ws2_32_org.WahCloseHandleHelper,@138")
#pragma comment(linker, "/export:WahCloseNotificationHandleHelper=ws2_32_org.WahCloseNotificationHandleHelper,@139")
#pragma comment(linker, "/export:WahCloseSocketHandle=ws2_32_org.WahCloseSocketHandle,@140")
#pragma comment(linker, "/export:WahCloseThread=ws2_32_org.WahCloseThread,@141")
#pragma comment(linker, "/export:WahCompleteRequest=ws2_32_org.WahCompleteRequest,@142")
#pragma comment(linker, "/export:WahCreateHandleContextTable=ws2_32_org.WahCreateHandleContextTable,@143")
#pragma comment(linker, "/export:WahCreateNotificationHandle=ws2_32_org.WahCreateNotificationHandle,@144")
#pragma comment(linker, "/export:WahCreateSocketHandle=ws2_32_org.WahCreateSocketHandle,@145")
#pragma comment(linker, "/export:WahDestroyHandleContextTable=ws2_32_org.WahDestroyHandleContextTable,@146")
#pragma comment(linker, "/export:WahDisableNonIFSHandleSupport=ws2_32_org.WahDisableNonIFSHandleSupport,@147")
#pragma comment(linker, "/export:WahEnableNonIFSHandleSupport=ws2_32_org.WahEnableNonIFSHandleSupport,@148")
#pragma comment(linker, "/export:WahEnumerateHandleContexts=ws2_32_org.WahEnumerateHandleContexts,@149")
#pragma comment(linker, "/export:WahInsertHandleContext=ws2_32_org.WahInsertHandleContext,@150")
#endif
#pragma comment(linker, "/export:___WSAFDIsSet=ws2_32_org.__WSAFDIsSet,@151")
#ifdef _WIN64
#pragma comment(linker, "/export:WSCWriteProviderOrder32=ws2_32_org.WSCWriteProviderOrder32,@152")
#pragma comment(linker, "/export:WahCloseApcHelper=ws2_32_org.WahCloseApcHelper,@153")
#pragma comment(linker, "/export:WahCloseHandleHelper=ws2_32_org.WahCloseHandleHelper,@154")
#pragma comment(linker, "/export:WahCloseNotificationHandleHelper=ws2_32_org.WahCloseNotificationHandleHelper,@155")
#pragma comment(linker, "/export:WahCloseSocketHandle=ws2_32_org.WahCloseSocketHandle,@156")
#pragma comment(linker, "/export:WahCloseThread=ws2_32_org.WahCloseThread,@157")
#pragma comment(linker, "/export:WahCompleteRequest=ws2_32_org.WahCompleteRequest,@158")
#pragma comment(linker, "/export:WahCreateHandleContextTable=ws2_32_org.WahCreateHandleContextTable,@159")
#pragma comment(linker, "/export:WahCreateNotificationHandle=ws2_32_org.WahCreateNotificationHandle,@160")
#pragma comment(linker, "/export:WahCreateSocketHandle=ws2_32_org.WahCreateSocketHandle,@161")
#pragma comment(linker, "/export:WahDestroyHandleContextTable=ws2_32_org.WahDestroyHandleContextTable,@162")
#pragma comment(linker, "/export:WahDisableNonIFSHandleSupport=ws2_32_org.WahDisableNonIFSHandleSupport,@163")
#pragma comment(linker, "/export:WahEnableNonIFSHandleSupport=ws2_32_org.WahEnableNonIFSHandleSupport,@164")
#pragma comment(linker, "/export:WahEnumerateHandleContexts=ws2_32_org.WahEnumerateHandleContexts,@165")
#pragma comment(linker, "/export:WahInsertHandleContext=ws2_32_org.WahInsertHandleContext,@166")
#pragma comment(linker, "/export:WahNotifyAllProcesses=ws2_32_org.WahNotifyAllProcesses,@167")
#pragma comment(linker, "/export:WahOpenApcHelper=ws2_32_org.WahOpenApcHelper,@168")
#pragma comment(linker, "/export:WahOpenCurrentThread=ws2_32_org.WahOpenCurrentThread,@169")
#pragma comment(linker, "/export:WahOpenHandleHelper=ws2_32_org.WahOpenHandleHelper,@170")
#pragma comment(linker, "/export:WahOpenNotificationHandleHelper=ws2_32_org.WahOpenNotificationHandleHelper,@171")
#pragma comment(linker, "/export:WahQueueUserApc=ws2_32_org.WahQueueUserApc,@172")
#pragma comment(linker, "/export:WahReferenceContextByHandle=ws2_32_org.WahReferenceContextByHandle,@173")
#pragma comment(linker, "/export:WahRemoveHandleContext=ws2_32_org.WahRemoveHandleContext,@174")
#pragma comment(linker, "/export:WahWaitForNotification=ws2_32_org.WahWaitForNotification,@175")
#pragma comment(linker, "/export:WahWriteLSPEvent=ws2_32_org.WahWriteLSPEvent,@176")
#pragma comment(linker, "/export:freeaddrinfo=ws2_32_org.freeaddrinfo,@177")
#pragma comment(linker, "/export:getaddrinfo=ws2_32_org.getaddrinfo,@178")
#pragma comment(linker, "/export:getnameinfo=ws2_32_org.getnameinfo,@179")
#pragma comment(linker, "/export:inet_ntop=ws2_32_org.inet_ntop,@180")
#pragma comment(linker, "/export:inet_pton=ws2_32_org.inet_pton,@181")
#else
#pragma comment(linker, "/export:WahNotifyAllProcesses=ws2_32_org.WahNotifyAllProcesses,@152")
#pragma comment(linker, "/export:WahOpenApcHelper=ws2_32_org.WahOpenApcHelper,@153")
#pragma comment(linker, "/export:WahOpenCurrentThread=ws2_32_org.WahOpenCurrentThread,@154")
#pragma comment(linker, "/export:WahOpenHandleHelper=ws2_32_org.WahOpenHandleHelper,@155")
#pragma comment(linker, "/export:WahOpenNotificationHandleHelper=ws2_32_org.WahOpenNotificationHandleHelper,@156")
#pragma comment(linker, "/export:WahQueueUserApc=ws2_32_org.WahQueueUserApc,@157")
#pragma comment(linker, "/export:WahReferenceContextByHandle=ws2_32_org.WahReferenceContextByHandle,@158")
#pragma comment(linker, "/export:WahRemoveHandleContext=ws2_32_org.WahRemoveHandleContext,@159")
#pragma comment(linker, "/export:WahWaitForNotification=ws2_32_org.WahWaitForNotification,@160")
#pragma comment(linker, "/export:WahWriteLSPEvent=ws2_32_org.WahWriteLSPEvent,@161")
#pragma comment(linker, "/export:freeaddrinfo=ws2_32_org.freeaddrinfo,@162")
#pragma comment(linker, "/export:getaddrinfo=ws2_32_org.getaddrinfo,@163")
#pragma comment(linker, "/export:getnameinfo=ws2_32_org.getnameinfo,@164")
#pragma comment(linker, "/export:inet_ntop=ws2_32_org.inet_ntop,@165")
#pragma comment(linker, "/export:inet_pton=ws2_32_org.inet_pton,@166")
#endif
#pragma comment(linker, "/export:WEP=ws2_32_org.WEP,@500")