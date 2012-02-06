import platform
test_paths = ['/opt/local/']

def set_options(opt):
	opt.tool_options("compiler_cxx")

def configure(conf):
	conf.check_tool("compiler_cxx")
	conf.check_tool("node_addon")

	if platform.architecture()[0][:2] == "64":
		conf.env.append_value('CXXFLAGS', ['-m64'])
		conf.env.append_value('LINKFLAGS', ['-m64'])

	conf.check(lib='svn_fs-1', libpath=build_paths('lib'))
	conf.check(lib='svn_fs_base-1', libpath=build_paths('lib'))
	conf.check(lib='svn_fs_fs-1', libpath=build_paths('lib'))
	conf.check(lib='svn_client-1', libpath=build_paths('lib'))
	conf.check(lib='svn_subr-1', libpath=build_paths('lib'))
	conf.check(lib='svn_repos-1', libpath=build_paths('lib'))
	conf.check(lib='svn_ra-1', libpath=build_paths('lib'))
	conf.check(lib='apr-1', libpath=build_paths('lib'))
	conf.check(lib='aprutil-1', libpath=build_paths('lib'));

def build(bld):
	obj = bld.new_task_gen("cxx", "shlib", "node_addon")
	if platform.architecture()[0][:2] == "64":
		obj.cxxflags = ["-g", "-m64", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE", "-Wall"]
		obj.ldflags = ['-m64']

	obj.lib = ['svn_client-1', 'apr-1', 'aprutil-1'];
	obj.libpath = build_paths('lib') ;
	obj.target = "nodesvn"
	obj.source = "src/svn.cc"
	obj.includes = build_paths('include') + build_paths('include/subversion-1') + build_paths('include/apr-1/') + build_paths('include/apr-1.0')

def all(bld):
	configure(bld);
	all(bld);

def build_paths(type):
	def normal(path):
		if path[-1] == '/':
			return path[:-1];
	
	return map(lambda path: normal(path)+'/'+type, test_paths[:]);
