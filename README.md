Node NetInfo addon
==================
This addon for Node.js allows you to get the system's DNS configuration under Windows.

Usage
=====
1. Drop the NodeWinNetInfo.node file (actually a DLL) into your app directory.
2. `var ni = require('./NodeWinNetInfo');`
3. `ni.getNetworkParams();`

You'll get something like this:

	{ hostName: 'daguej',
	  domainName: '',
	  dnsServers:
	   [ '8.8.8.8',
		 '8.8.4.4', ],
	  dnsEnabled: false,
	  nodeType: 4,
	  domain: '',
	  arpProxyEnabled: false,
	  routingEnabled: false,
	  dhcpScope: '' }