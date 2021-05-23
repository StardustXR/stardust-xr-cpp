project('stardustxr', 'cpp', default_options : ['cpp_std=c++17'])

pkg = import('pkgconfig')
libstardustxr = dependency('stardustxr-server', required: true)
stereokit = dependency('StereoKitC', required: true)

src_includes = include_directories('src')
thirdparty_includes = include_directories('thirdparty')
includes = [src_includes, thirdparty_includes]

subdir('src')

executable(
	'stardustxr',
	'src/main.cpp',
	include_directories: includes,
	link_args: '-lflatbuffers',
	dependencies: [
		libstardustxr,
		stereokit,
# After here are all the internal dependencies
		nodetypes,
		scenegraph,
		tests,
		util,
	],
)
