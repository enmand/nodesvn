#include "svn.h"
#include <sys/stat.h>

void SVN::Initiaize(Handle<Object> target)
{
	HandleScope scope;

	Local<FunctionTemplate> t = FunctionTemplate::New(New);

	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(String::NewSymbol("SVN"));

	NODE_SET_PROTOTYPE_METHOD(t, "open", Open);

	t->PrototypeTemplate()->SetAccessor(String::NewSymbol("path"), PathGetter);

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

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	SVN::Initiaize(target);
}
