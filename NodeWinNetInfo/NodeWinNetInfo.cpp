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
	void Get(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		FIXED_INFO *pFixedInfo;
		ULONG ulOutBufLen;
		DWORD dwRetVal;
		IP_ADDR_STRING *pDNS;

		pFixedInfo = (FIXED_INFO *) MALLOC(sizeof (FIXED_INFO));
		if (pFixedInfo == NULL) {
			isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error allocating memory")));
			args.GetReturnValue().Set(Undefined(isolate));
			return;
		}
		ulOutBufLen = sizeof (FIXED_INFO);

		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			FREE(pFixedInfo);
			pFixedInfo = (FIXED_INFO *) MALLOC(ulOutBufLen);
			if (pFixedInfo == NULL) {
				isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error allocating memory")));
				args.GetReturnValue().Set(Undefined(isolate));
				return;
			}
		}

		
		if (dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) {		
			Local<Object> r = Object::New(isolate);

			r->Set(String::NewFromUtf8(isolate, "hostName"), String::NewFromUtf8(isolate, pFixedInfo->HostName));
			r->Set(String::NewFromUtf8(isolate, "domainName"), String::NewFromUtf8(isolate, pFixedInfo->DomainName));
			
			Local<Array> dns = Array::New(isolate);
			uint32_t i = 0;

			dns->Set(i++, String::NewFromUtf8(isolate, pFixedInfo->DnsServerList.IpAddress.String));
			pDNS = pFixedInfo->DnsServerList.Next;
			while (pDNS) {
				dns->Set(i++, String::NewFromUtf8(isolate, pDNS->IpAddress.String));
				pDNS = pDNS->Next;
			}
				
			r->Set(String::NewFromUtf8(isolate, "dnsServers"), dns);
			r->Set(String::NewFromUtf8(isolate, "dnsEnabled"), Boolean::New(isolate, pFixedInfo->EnableDns != 0));
			r->Set(String::NewFromUtf8(isolate, "nodeType"), Number::New(isolate, pFixedInfo->NodeType));
			r->Set(String::NewFromUtf8(isolate, "domain"), String::NewFromUtf8(isolate, pFixedInfo->DomainName));
			r->Set(String::NewFromUtf8(isolate, "arpProxyEnabled"), Boolean::New(isolate, pFixedInfo->EnableProxy != 0));
			r->Set(String::NewFromUtf8(isolate, "routingEnabled"), Boolean::New(isolate, pFixedInfo->EnableRouting != 0));
			r->Set(String::NewFromUtf8(isolate, "dhcpScope"), String::NewFromUtf8(isolate, pFixedInfo->ScopeId));


			if (pFixedInfo) FREE(pFixedInfo);

			args.GetReturnValue().Set(r);
			return;
		} else {
			if (pFixedInfo) FREE(pFixedInfo);
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "GetNetworkParams failed with error " + dwRetVal)));
			args.GetReturnValue().Set(Undefined(isolate));
			return;
		}

		if (pFixedInfo)
			FREE(pFixedInfo);

		args.GetReturnValue().Set(Undefined(isolate));
		return;
	}

	void init(v8::Local<v8::Object> target) {
		NODE_SET_METHOD(target, "getNetworkParams", Get);
	}

	NODE_MODULE(NodeWinNetInfo, init)
}