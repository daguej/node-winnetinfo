Node NetInfo addon
==================
This addon for Node.js allows you to get the system's DNS configuration under Windows.

It doesn't work on *nix, where you can simply read `/etc/resolv.conf` to get the system's DNS servers.

Installation
============
This project is available on npm.  Since it is a native (C++) addon, you'll need [everything node-gyp needs](https://github.com/TooTallNate/node-gyp#installation) to build projects, which includes Python 2.7 and MS VC++.

    > npm install winnetinfo

Usage
=====
1. `var ni = require('winnetinfo');`
2. `ni.getNetworkParams();`

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
