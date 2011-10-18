#include "svn.h"

SVN::SVN(const char *config = NULL) 
{
	HandleScope scope;

	svn_error_t *err;

	apr_initialize();

	this->pool = svn_pool_create(NULL); // Create the APR memory pool
	
	// Create our client context
	if ( (err = svn_client_create_context(&this->ctx, this->pool)) )
	{
		this->error(err);
		svn_pool_destroy(this->pool);
		this->pool = NULL;
		return;
	}

	// If we have a config path defined, that will be used. Otherwise, we're set
	// without one
	if( (err = svn_config_get_config(&this->ctx->config, config, this->pool)) )
	{
		this->error(err);
		svn_pool_destroy(this->pool);
		this->pool = NULL;
		return;

	}
}

void SVN::InitModule(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> t = FunctionTemplate::New(New);
	ct = Persistent<FunctionTemplate>::New(t);
	ct->InstanceTemplate()->SetInternalFieldCount(1);
	ct->SetClassName(String::NewSymbol("SVN"));

	NODE_SET_PROTOTYPE_METHOD(ct, "authenticate", __authenticate);
	NODE_SET_PROTOTYPE_METHOD(ct, "cat", __cat);

	target->Set(String::NewSymbol("SVN"), ct->GetFunction());
}

Handle<Value> SVN::New(const Arguments &args)
{
	HandleScope scope;
	SVN *svn = new SVN();

	String::AsciiValue config(args[0]->ToString());
	svn = new SVN(*config);
	
	svn->Wrap(args.This());

	return args.This();
}

Handle<Value> SVN::__cat(const Arguments &args)
{
	HandleScope scope;
	Local<String> url;
	apr_size_t size;
	apr_pool_t *subpool;
	svn_error_t *error;
	svn_stream_t *out;
	svn_stringbuf_t *buf;
	svn_opt_revision_t revision;

	revision.value.number = svn_opt_revision_unspecified;
}

Handle<Value> SVN::__authenticate(const Arguments &args)
{
	HandleScope scope;

	SVN *svn = ObjectWrap::Unwrap<SVN>(args.This());

	if(args.Length() < 2 && (args[0]->IsString() && args[1]->IsString()))
	{
		return ThrowException(Exception::Error(
				String::New("You must provide both the username and the password for authentication")
			));
	}

	String::AsciiValue username(args[0]->ToString());
	String::AsciiValue password(args[0]->ToString());
	svn->authenticate(*username, *password);

	return Undefined();
}

void SVN::authenticate(const char *user, const char *secret)
{
	svn_auth_provider_object_t *providers;
	
	providers = apr_array_make(this->pool, 2, sizeof(svn_auth_provider_object_t*));
}

Handle<String> SVN::error(svn_error_t *error)
{
	HandleScope scope;
	svn_error_t *err_it = error;
	Local<String> strerror = String::New("SVN Error Occured: ");

	while( err_it )
	{
		char buffer[256];
		svn_strerror(err_it->apr_err, buffer, sizeof(buffer));
		String::Concat(strerror, String::New(" ( "));
		String::Concat(strerror, String::New(buffer));
		String::Concat(strerror, String::New(" ) "));
		String::Concat(strerror, String::New(err_it->message));
	}
	return scope.Close(strerror);
}

Persistent<FunctionTemplate> SVN::ct;

extern "C" void init(Handle<Object> target)
{
	HandleScope scope;

	SVN::InitModule(target);
}
