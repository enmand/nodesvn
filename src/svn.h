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
#include <sys/stat.h>

using namespace node;
using namespace v8;

class SVN : public ObjectWrap
{
public:
	static Persistent<FunctionTemplate> ct;

	SVN(const char *config);
	static void InitModule(Handle<Object> target); // V8/Node initializer
	static Handle<Value> New(const Arguments &args); // 'new' construct
	
	void init_auth();
	void simple_authentication(const char *username, const char *password);

	~SVN()
	{
		svn_pool_destroy(this->pool);
		this->pool = NULL;
		apr_terminate();
	}

protected:
	// SVN-defined functions
	static Handle<Value> __checkout(const Arguments &args);
	static Handle<Value> __import(const Arguments &args);
	static Handle<Value> __blame(const Arguments &args);
	static Handle<Value> __cat(const Arguments &args);
	static Handle<Value> __copy(const Arguments &args);
	static Handle<Value> __del(const Arguments &args);
	static Handle<Value> __exp(const Arguments &args);
	static Handle<Value> __info(const Arguments &args);
	static Handle<Value> __list(const Arguments &args);
	static Handle<Value> __log(const Arguments &args);
	static Handle<Value> __mkdir(const Arguments &args);
	static Handle<Value> __move(const Arguments &args);
	static Handle<Value> __add(const Arguments &args);
	static Handle<Value> __cleanup(const Arguments &args);
	static Handle<Value> __commit(const Arguments &args);
	static Handle<Value> __revert(const Arguments &args);
	static Handle<Value> __status(const Arguments &args);
	static Handle<Value> __update(const Arguments &args);

	// SVN baton authentication
	static Handle<Value> __authenticate(const Arguments &args);

	// Class methods

	// Accessors

	// Inceptors
private:
	apr_pool_t *pool;
	svn_client_ctx_t *ctx;

	const char* config; // Path to config	
	Handle<String> error(svn_error_t *error);
};

#endif
