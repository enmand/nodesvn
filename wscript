test_paths = ['/', '/usr/', '/usr/local/', '/opt/local/']

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.check(lib='svn_fs-1', libpath=build_paths('lib'))
  conf.check(lib='svn_fs_base-1', libpath=build_paths('lib'))
  conf.check(lib='svn_fs_fs-1', libpath=build_paths('lib'))
  conf.check(lib='svn_client-1', libpath=build_paths('lib'))
  conf.check(lib='svn_subr-1', libpath=build_paths('lib'))
  conf.check(lib='svn_repos-1', libpath=build_paths('lib'))
  conf.check(lib='svn_ra-1', libpath=build_paths('lib'))


def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.cxxflags = ["-g", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE", "-Wall"]
  obj.uselib = "svn"
  obj.target = "nodesvn"
  obj.source = "src/svn.cc"
  obj.includes = build_paths('include') + build_paths('include/subversion-1') + build_paths('include/apr-1/')

def all(bld):
  print "Use 'node-waf configure build'"
  pass;
  
def build_paths(type):
  def normal(path):
    if path[-1] == '/':
      return path[:-1];
  
  return map(lambda path: normal(path)+'/'+type, test_paths[:]);
