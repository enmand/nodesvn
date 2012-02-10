# NodeJS SVN bindings

This module is SVN bindings for NodeJS. You can use this module to interact with SVN within your NodeJS project.

# Installation

Installation of this module is simple. In the root directory

	node-waf configure build install
	
# Usage

## Filesystem File Contents

To get the contents of a file directly from the Subversion filesystem (using svn_fs):


	SVN = require('nodesvn').SVN;
	client = new SVN();

	client.open('/path/to/svn/filesystem')
	file_contents = client.file_contents('path_to_file_in_filesystem')


## cat file

To get the contents of the file, as you might if you don't have direct access to the file system (e.g. through your SVN client on your OS, using svn_ra) use ``cat``

	SVN = require('nodesvn').SVN
	client = new SVN();

	client.cat('type://path/to/repository/file')

# Future Developments

This library was initially developed for a specific purpose, however it _will be developed further_. If you would like to submit patches, please feel free to send us _pull requests_.
