#include "svn.h"

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

	if(args.Length() == 0 || !args[0]->IsString())
	{
		return ThrowException(String::New("Must provide repository path as an arguement"));
	}

	String::Utf8Value path(args[0]->ToString());
	SVN *svn = new SVN(*path);
	svn->Wrap(args.This());

	return args.This();
}

Handle<Value> SVN::PathGetter(Local<String> property, const AccessorInfo& info)
{
	SVN *svn = ObjectWrap::Unwrap<SVN>(info.This());
	assert(svn);

	HandleScope scope;

	return scope.Close(String::NewSymbol("DerpKyleHerp"));
}

Handle<Value> SVN::Open(const Arguments &args)
{
	SVN *svn = ObjectWrap::Unwrap<SVN>(args.This());
	assert(svn);

	return Null();
}

extern "C" void
init (Handle<Object> target)
{
	HandleScope scope;
	SVN::Initiaize(target);
}
