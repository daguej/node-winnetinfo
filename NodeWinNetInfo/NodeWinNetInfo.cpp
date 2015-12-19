// NodeWinNetInfo.cpp : Defines the exported functions for the DLL application.
//

#include <nan.h>

#include <WinSock2.h>
#include <IPHlpApi.h>
#include <Windows.h>
#pragma comment(lib, "IPHLPAPI.lib")


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


namespace WinNetInfo {
	using namespace v8;

	void Get(const Nan::FunctionCallbackInfo<Value>& info) {
		FIXED_INFO *pFixedInfo;
		ULONG ulOutBufLen;
		DWORD dwRetVal;
		IP_ADDR_STRING *pDNS;

		pFixedInfo = (FIXED_INFO *) MALLOC(sizeof (FIXED_INFO));
		if (pFixedInfo == NULL) {
			Nan::ThrowError("Error allocating memory");
			info.GetReturnValue().Set(Nan::Undefined());
			return;
		}
		ulOutBufLen = sizeof (FIXED_INFO);

		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			FREE(pFixedInfo);
			pFixedInfo = (FIXED_INFO *) MALLOC(ulOutBufLen);
			if (pFixedInfo == NULL) {
				Nan::ThrowError("Error allocating memory");
				info.GetReturnValue().Set(Nan::Undefined());
				return;
			}
		}

		
		if (dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) {		
			Local<Object> r = Nan::New<Object>();

			r->Set(Nan::New("hostName").ToLocalChecked(), Nan::New(pFixedInfo->HostName).ToLocalChecked());
			r->Set(Nan::New("domainName").ToLocalChecked(), Nan::New(pFixedInfo->DomainName).ToLocalChecked());
			
			Local<Array> dns = Nan::New<Array>();
			uint32_t i = 0;

			dns->Set(i++, Nan::New(pFixedInfo->DnsServerList.IpAddress.String).ToLocalChecked());
			pDNS = pFixedInfo->DnsServerList.Next;
			while (pDNS) {
				dns->Set(i++, Nan::New(pDNS->IpAddress.String).ToLocalChecked());
				pDNS = pDNS->Next;
			}

			r->Set(Nan::New("dnsServers").ToLocalChecked(), dns);
			r->Set(Nan::New("dnsEnabled").ToLocalChecked(), Nan::New(pFixedInfo->EnableDns != 0));
			r->Set(Nan::New("nodeType").ToLocalChecked(), Nan::New(pFixedInfo->NodeType));
			r->Set(Nan::New("domain").ToLocalChecked(), Nan::New(pFixedInfo->DomainName).ToLocalChecked());
			r->Set(Nan::New("arpProxyEnabled").ToLocalChecked(), Nan::New(pFixedInfo->EnableProxy != 0));
			r->Set(Nan::New("routingEnabled").ToLocalChecked(), Nan::New(pFixedInfo->EnableRouting != 0));
			r->Set(Nan::New("dhcpScope").ToLocalChecked(), Nan::New(pFixedInfo->ScopeId).ToLocalChecked());


			if (pFixedInfo) FREE(pFixedInfo);

			info.GetReturnValue().Set(r);
			return;
		} else {
			if (pFixedInfo) FREE(pFixedInfo);
			Nan::ThrowTypeError("GetNetworkParams failed with error " + dwRetVal);
			info.GetReturnValue().Set(Nan::Undefined());
			return;
		}

		if (pFixedInfo)
			FREE(pFixedInfo);

		info.GetReturnValue().Set(Nan::Undefined());
		return;
	}

	void init(Local<Object> exports) {
		exports->Set(Nan::New("getNetworkParams").ToLocalChecked(),
			Nan::New<FunctionTemplate>(Get)->GetFunction());
	}

	NODE_MODULE(NodeWinNetInfo, init)
}