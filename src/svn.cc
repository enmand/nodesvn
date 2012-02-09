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

	// Add our providers for authentication and set up
	// our client for proper authentication
	this->init_auth();
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
	NODE_SET_PROTOTYPE_METHOD(ct, "open", __open);
	NODE_SET_PROTOTYPE_METHOD(ct, "file_contents", __file_contents);

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

Handle<Value> SVN::__open(const Arguments &args)
{
	HandleScope scope;
	SVN *svn = ObjectWrap::Unwrap<SVN>(args.This());
	svn_error_t *err;

	if (args.Length() != 1 || !args[0]->IsString())
	{
		ERROR("Please enter a filesystem path");
	}

	String::Utf8Value path(args[0]->ToString());
	svn->_lock.pool = svn_pool_create(svn->pool);

	if( (err = svn_repos_open(&svn->_lock.repos, *path, svn->_lock.pool)) )
	{
		svn_pool_destroy(svn->_lock.pool);
		ERROR(svn->error(err));
	}

	return scope.Close(Boolean::New(true));
}

Handle<Value> SVN::__file_contents(const Arguments &args)
{
	HandleScope scope;
	SVN *svn = ObjectWrap::Unwrap<SVN>(args.This());

	svn_revnum_t rev;
	svn_revnum_t head;
	svn_error_t *err;
	svn_fs_root_t *root;

	switch(args.Length())
	{
		case 2:
			if(args[1]->IsNumber())
			{
				rev = args[1]->ToNumber()->Value();
			} else
			{
				rev = -1;
			}
		case 1:
			if( !args[0]->IsString() )
			{
				ERROR("Please provide a file to get the contents of");
			}
	}

	String::Utf8Value path(args[0]->ToString());
	
	if (svn->_lock.repos == NULL)
	{
		ERROR("Specify a repository with subversion.open");
	}

	svn_fs_t *fs = svn_repos_fs(svn->_lock.repos);


	if ( (err = svn_fs_youngest_rev(&head, fs, svn->_lock.pool) ))
	{
		ERROR(svn->error(err));
	}

	if (rev > head || rev == -1)
	{
		rev = head;
	}


	if( (err = svn_fs_revision_root(&root, fs, rev, svn->_lock.pool) ))
	{
		ERROR(svn->error(err));
	}
	
	svn_stream_t *contents;
	svn_filesize_t *length = new svn_filesize_t;

	if ( (err = svn_fs_file_length(length, root, *path, svn->_lock.pool) ))
	{
		ERROR(svn->error(err));
	}

	if ( (err = svn_fs_file_contents(&contents, root , *path, svn->_lock.pool) ))
	{
		ERROR(svn->error(err));
	}

	char *out = new char[(apr_size_t)*length];
	svn_stream_read(contents, out, (apr_size_t*)length);
	return scope.Close(String::New(out));
}

Handle<Value> SVN::__cat(const Arguments &args)
{
	HandleScope scope;
	Local<String> url;
	SVN *svn = ObjectWrap::Unwrap<SVN>(args.This());
	svn_opt_revision_t revision;
	apr_pool_t *subpool = svn_pool_create(svn->pool);
	svn_error_t *err;
	svn_opt_revision_t peg_revision = { svn_opt_revision_unspecified };


	switch(args.Length())
	{
		case 2:
			if(args[1]->IsNumber())
			{
				revision.value.number = args[1]->ToNumber()->Value();
				revision.kind = svn_opt_revision_number;
			} else
			{
				revision.kind = svn_opt_revision_head;
			}
		
		case 1:
			if(args[0]->IsString())
				url = args[0]->ToString();
			else
				ERROR("Your URL path must be a string");
			revision.kind = svn_opt_revision_head;
		break;
		default:
			ERROR("Expected: url[,revision]");
	}

	String::Utf8Value url_utf(url);
	
	// pool and buffers setup
	svn_stringbuf_t *buf = svn_stringbuf_create("", subpool);
	svn_stream_t *out = svn_stream_from_stringbuf(buf, subpool);

	if( (err = svn_client_cat2(out, *url_utf, &peg_revision, &revision, svn->ctx, subpool)) )
	{
		svn_pool_destroy(subpool);
		subpool = NULL;
		ERROR(svn->error(err));
	}
	return scope.Close(String::New(buf->data, buf->len));
}

// We currently only push the "simple provider" for basic user/pass
// authentication. Others may follow.
Handle<Value> SVN::__authenticate(const Arguments &args)
{
	HandleScope scope;
	SVN *svn = ObjectWrap::Unwrap<SVN>(args.This());

	// If we have a username and password passed in, setting those
	// to the SVN_AUTH_PARAM_DEFAULT_USERNAME and SVN_AUTH_PARAM_DEFAULT_PASSWORD
	// will allow us to use the "simple providers" from Subversion.
	if(args.Length() == 2 && (args[0]->IsString() && args[1]->IsString()))
	{
		String::Utf8Value username(args[0]->ToString());
		String::Utf8Value password(args[1]->ToString());
		svn->simple_authentication(*username, *password);
		return scope.Close(Boolean::New(true));
	}

	return scope.Close(Boolean::New(false));
}

void SVN::init_auth()
{
	svn_auth_provider_object_t *_provider;
	svn_auth_baton_t *auth_baton;

	apr_array_header_t *providers = apr_array_make(this->pool, 10, sizeof(svn_auth_provider_object_t *));

#ifdef DARWIN
	svn_auth_get_keychain_simple_provider(&_provider, this->pool);
	*(svn_auth_provider_object_t **)apr_array_push(providers) = _provider;
#endif

	svn_auth_get_simple_provider2(&_provider, NULL, NULL, this->pool);
	*(svn_auth_provider_object_t **)apr_array_push(providers) = _provider;

	svn_auth_get_username_provider(&_provider, this->pool);
	*(svn_auth_provider_object_t **)apr_array_push(providers) = _provider;

	svn_auth_open(&auth_baton, providers, this->pool);
	
	svn_auth_set_parameter(auth_baton, SVN_AUTH_PARAM_NON_INTERACTIVE, "");
	this->ctx->auth_baton = auth_baton;
}


void SVN::simple_authentication(const char *username, const char *password)
{
	svn_auth_set_parameter(this->ctx->auth_baton, 
				apr_pstrdup(this->pool, SVN_AUTH_PARAM_DEFAULT_USERNAME),
				apr_pstrdup(this->pool, username));
	svn_auth_set_parameter(this->ctx->auth_baton,
				apr_pstrdup(this->pool, SVN_AUTH_PARAM_DEFAULT_PASSWORD),
				apr_pstrdup(this->pool, password));
}

const char* SVN::error(svn_error_t *error)
{
	svn_error_t *err_it = error;
	Local<String> strerror = String::New("SVN Error Occured: ");

	while( err_it )
	{
		char buffer[256];
		svn_strerror(err_it->apr_err, buffer, sizeof(buffer));
		strerror = String::Concat(strerror, String::New(" ( "));
		strerror = String::Concat(strerror, String::New(buffer));
		strerror = String::Concat(strerror, String::New(" ) "));
		strerror = String::Concat(strerror, String::New(err_it->message));

		err_it = err_it->child;
		if(err_it != NULL)
		{
			strerror = String::Concat(strerror, String::New("\n"));
		}
	}

	String::Utf8Value err(strerror);
	return *err;
}

Persistent<FunctionTemplate> SVN::ct;

extern "C" void init(Handle<Object> target)
{
	HandleScope scope;

	SVN::InitModule(target);
}
