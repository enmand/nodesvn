nodesvn = require('./build/default/nodesvn');

var subversion = nodesvn.SVN;

exports.SVN = function(path)
{
	return new subversion(path);
}