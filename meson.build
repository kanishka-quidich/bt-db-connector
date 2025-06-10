project('quidich-db', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

cpp = meson.get_compiler('cpp')
cpp_driver_dep = cpp.find_library('cassandra', dirs : ['/usr/local/lib/x86_64-linux-gnu', '/usr/local/lib', '/usr/lib', '/usr/lib64'], required : true)
zmq_dep = cpp.find_library('zmq', required : true)

src_files = [
  'src/DBApi.cpp',
]

quidich_db_lib = library('quidich-db',
  src_files,
  dependencies : [cpp_driver_dep, zmq_dep],
  include_directories : include_directories('.', '/usr/local/include'))

test_exe = executable('db_test',
  'src/test.cpp',
  dependencies : [cpp_driver_dep, zmq_dep],
  link_with : quidich_db_lib,
  include_directories : include_directories('.', '/usr/local/include'))

