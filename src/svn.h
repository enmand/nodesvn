/**
 * NodeJS SVN bindings
 *
 * This bindings allow filesystem-level access to Subversion
 * reposotories. This will allow you to interact with Subversion
 * without requiring a local working copy.
 *
 * This module will be expanded to be a full and functioning SVN
 * library for NodeJS
 *
 * @author Daniel Enman
 */
#ifndef SVN_H
#define SVN_H

// Node/V8
#include <node.h>
#include <node_events.h>
#include <v8.h>

// SVN Includes
#include <svn_repos.h>
#include <svn_ra.h>
#include <svn_fs.h>
#include <svn_pools.h>
#include <svn_path.h>
#include <svn_utf.h>

// Other
#include <stdlib.h>

#define PATH String::NewSymbol("path");

using namespace node;
using namespace v8;

class SVN : ObjectWrap
{
public:
	SVN(const char *path) : path(path) {
	}

	~SVN() {
	}
	static void Initiaize(Handle<Object> target);
protected:
	static Handle<Value> New(const Arguments &args);

	// Class methods
	static Handle<Value> Open(const Arguments &args);
	static Handle<Value> Read(const Arguments &args);

	// Accessors
	static Handle<Value> PathGetter(Local<String> property, const AccessorInfo& info);

	// Inceptors
private:
	const char *path;
	apr_pool_t *pool;
};

#endif
