#include "svn.h"
#include "svnfs.h"

class SVN : ObjectWrap
{
protected:
	char * repo_path;
};
