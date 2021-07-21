project('stardustxr', 'cpp', default_options : ['cpp_std=c++17'])

cmake = import('cmake')
pkg = import('pkgconfig')
libstardustxr = dependency('stardustxr-server', required: true)

stereokit_options = cmake.subproject_options()
stereokit_options.add_cmake_defines({'SK_LINUX_EGL': true, 'SK_BUILD_TESTS': false, 'SK_SYSTEMS_PHYSICS': false})
stereokit_options.add_cmake_defines({'CMAKE_CXX_FLAGS': '-Wno-all'})
stereokit_options.set_install(false)
stereokit_project = cmake.subproject('StereoKit', options: stereokit_options)
stereokit = stereokit_project.dependency('StereoKitC')

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
		core,
		interfaces,
		nodetypes,
		tests,
		util,
	],
)
