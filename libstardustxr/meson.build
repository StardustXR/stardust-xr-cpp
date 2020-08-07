project('stardustxr-godot', 'cpp')

godot_cpp = meson.get_compiler('cpp').find_library('libgodot-cpp.linux.debug.64', dirs: join_paths(meson.current_source_dir(), 'godot-cpp', 'bin'))
libstardustxr = dependency('stardustxr-server')

include = include_directories('include', 'godot-cpp/include', 'godot-cpp/include/core', 'godot-cpp/include/gen', 'godot-cpp/godot_headers')
sources = ['src/gdlibrary.cpp', 'src/messengermanager.cpp']
dependencies = [godot_cpp, libstardustxr]

shared_library('stardustxr-godot', sources, include_directories: include, dependencies: dependencies, link_args: '-lflatbuffers')
