// NodeWinNetInfo.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <v8.h>
using namespace v8;
#pragma comment(lib, "node")
#include <node.h>


#include <WinSock2.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib, "IPHLPAPI.lib")


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


namespace WinNetInfo {
	Handle<Value> Get(const Arguments &args) {
		HandleScope scope;

		FIXED_INFO *pFixedInfo;
		ULONG ulOutBufLen;
		DWORD dwRetVal;
		IP_ADDR_STRING *pDNS;

		pFixedInfo = (FIXED_INFO *) MALLOC(sizeof (FIXED_INFO));
		if (pFixedInfo == NULL) {
			ThrowException(Exception::Error(String::New("Error allocating memory")));
			return scope.Close(Undefined());
		}
		ulOutBufLen = sizeof (FIXED_INFO);

		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			FREE(pFixedInfo);
			pFixedInfo = (FIXED_INFO *) MALLOC(ulOutBufLen);
			if (pFixedInfo == NULL) {
				ThrowException(Exception::Error(String::New("Error allocating memory")));
				return scope.Close(Undefined());
			}
		}

		
		if (dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) {		
			Local<Object> r = Object::New();

			r->Set(String::NewSymbol("hostName"), String::NewSymbol(pFixedInfo->HostName));
			r->Set(String::NewSymbol("domainName"), String::NewSymbol(pFixedInfo->DomainName));
			
			Local<Array> dns = Array::New();
			//dns->
			uint32_t i = 0;

			dns->Set(i++, String::NewSymbol(pFixedInfo->DnsServerList.IpAddress.String));
			pDNS = pFixedInfo->DnsServerList.Next;
			while (pDNS) {
				dns->Set(i++, String::NewSymbol(pDNS->IpAddress.String));
				pDNS = pDNS->Next;
			}
				
			r->Set(String::NewSymbol("dnsServers"), dns);
			r->Set(String::NewSymbol("dnsEnabled"), Boolean::New(pFixedInfo->EnableDns));
			r->Set(String::NewSymbol("nodeType"), Number::New(pFixedInfo->NodeType));
			r->Set(String::NewSymbol("domain"), String::NewSymbol(pFixedInfo->DomainName));
			r->Set(String::NewSymbol("arpProxyEnabled"), Boolean::New(pFixedInfo->EnableProxy));
			r->Set(String::NewSymbol("routingEnabled"), Boolean::New(pFixedInfo->EnableRouting));
			r->Set(String::NewSymbol("dhcpScope"), String::NewSymbol(pFixedInfo->ScopeId));


			if (pFixedInfo) FREE(pFixedInfo);
			return scope.Close(r);

		} else {
			if (pFixedInfo) FREE(pFixedInfo);
			ThrowException(Exception::TypeError(String::New("GetNetworkParams failed with error " + dwRetVal)));
			return scope.Close(Undefined());
		}

		if (pFixedInfo)
			FREE(pFixedInfo);

		return scope.Close(Undefined());

	}
}

extern "C" void NODE_EXTERN init (Handle<Object> target)
{
	HandleScope scope;
	Local<FunctionTemplate> get = FunctionTemplate::New(WinNetInfo::Get);
	
	target->Set(String::NewSymbol("getNetworkParams"), get->GetFunction());
}

NODE_MODULE(NodeWinNetInfo, init)