Node NetInfo addon
==================
This addon for Node.js allows you to get the system's DNS configuration under Windows.  [Download a compiled binary here](https://github.com/downloads/daguej/node-winnetinfo/NodeWinNetInfo.node) if you're in a hurry (and taking candy from strangers is your thing).

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