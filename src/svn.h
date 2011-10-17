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
#include <svn_wc.h>
#include <svn_pools.h>
#include <svn_path.h>
#include <svn_utf.h>
#include <svn_client.h>

// Other
#include <stdlib.h>

using namespace node;
using namespace v8;

class SVN : ObjectWrap
{
public:
	SVN() {}
	static void Initiaize(Handle<Object> target); // V8/Node initializer

	static Handle<Value> Open(const Arguments &args);

	// SVN-defined functions
	static Handle<Value> checkout(const Arguments &args);
	static Handle<Value> import(const Arguments &argss);
	static Handle<Value> blame(const Arguments &args);
	static Handle<Value> cat(const Arguments &args);
	static Handle<Value> copy(const Arguments &args);
	static Handle<Value> del(const Arguments &args);
	static Handle<Value> exp(const Arguments &args);
	static Handle<Value> info(const Arguments &args);
	static Handle<Value> list(const Arguments &args);
	static Handle<Value> log(const Arguments &args);
	static Handle<Value> mkdir(const Arguments &args);
	static Handle<Value> move(const Arguments &args);
	static Handle<Value> add(const Arguments &args);
	static Handle<Value> cleanup(const Arguments &args);
	static Handle<Value> commit(const Arguments &args);
	static Handle<Value> revert(const Arguments &args);
	static Handle<Value> status(const Arguments &args);
	static Handle<Value> update(const Arguments &args);


	~SVN() {}
protected:
	static Handle<Value> New(const Arguments &args); // 'new' construct

	// Class methods

	// Accessors

	// Inceptors
private:
	apr_pool_t *pool;
	svn_client_ctx_t *ctx;
	
	Handle<String> error(svn_error_t *error);
};

#endif
