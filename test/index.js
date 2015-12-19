var assert = require('assert');
var util = require('util');
var wni = require('..');

var params = wni.getNetworkParams();
//console.info(params);
assert.equal('string', typeof params.hostName);
assert.equal('string', typeof params.domainName);
assert(util.isArray(params.dnsServers));
assert.equal('boolean', typeof params.dnsEnabled);
assert.equal('number', typeof params.nodeType);
assert.equal('string', typeof params.domain);
assert.equal('boolean', typeof params.arpProxyEnabled);
assert.equal('boolean', typeof params.routingEnabled);
assert.equal('string', typeof params.dhcpScope);
