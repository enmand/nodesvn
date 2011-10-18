nodesvn = require('./build/default/nodesvn');

var subversion = nodesvn.SVN;

function SVN()
{
	if(!this.client)
	{
		this.client = new nodesvn.SVN();
	}
}

exports.SVN = SVN;
