nodesvn = require('./build/default/nodesvn');

var subversion = nodesvn.SVN;

exports.createClient = function()
{
	_svn = new SVN();
	return _svn.client;
}

function SVN()
{
	if(!this.client)
	{
		this.client = new nodesvn.SVN();
	}
}

exports.SVN = SVN;
