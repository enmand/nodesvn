#include "svn.h"
#include <sys/stat.h>

void SVN::Initiaize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> t = FunctionTemplate::New(New);

	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(String::NewSymbol("SVN"));

	target->Set(String::NewSymbol("SVN"), t->GetFunction());
}

Handle<Value> SVN::New(const Arguments &args)
{
	HandleScope scope;

	SVN *svn = new SVN();
	svn->Wrap(args.This());

	return args.This();
}

Handle<Value> SVN::Open(const Arguments &args)
{
	HandleScope scope;
	struct stat _wc_config;

	if(args.Length() == 0 && !args[0]->IsString())
	{
		return ThrowException(String::New("You must provide a valid Working Copy path"));
	}

	String::Utf8Value path(args[0]->ToString());

	if(stat(*path, &_wc_config) < 0)
	{
		return ThrowException(String::New("Working copy path not found"));
	}

	return args.This();
}

Handle<Value> SVN::cat(const Arguments &args)
{
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

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	SVN::Initiaize(target);
}
