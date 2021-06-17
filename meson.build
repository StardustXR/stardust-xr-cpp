project('stardustxr', 'cpp', default_options : ['cpp_std=c++17'])

cmake = import('cmake')
pkg = import('pkgconfig')
libstardustxr = dependency('stardustxr-server', required: true)

stereokit_project = cmake.subproject('StereoKit')
stereokit_options = cmake.subproject_options()
stereokit_options.set_install(false)
stereokit = stereokit_project.dependency('StereoKitC', options: stereokit_options)

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
