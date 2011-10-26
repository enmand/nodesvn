nodesvn = require('./build/Release/nodesvn');

function SVN()
{
	if(!this.client)
	{
		this.client = new nodesvn.SVN();
	}
	return this.client;
}

exports.SVN = SVN;
