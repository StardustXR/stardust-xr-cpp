project('stardustxr', 'cpp', default_options: ['cpp_std=c++17'], version:'0.55.0')

cmake = import('cmake')
pkg = import('pkgconfig')
libstardustxr = dependency('stardustxr-server', required: true)

stereokit_options = cmake.subproject_options()
stereokit_options.add_cmake_defines({'SK_LINUX_EGL': true, 'SK_BUILD_TESTS': false, 'SK_SYSTEMS_PHYSICS': false, 'SK_BUILD_SHARED_LIBS': false})
stereokit_options.add_cmake_defines({'CMAKE_CXX_FLAGS': '-Wno-all'})
stereokit_options.set_install(false)
stereokit_project = cmake.subproject('StereoKit', options: stereokit_options)
stereokit = stereokit_project.dependency('StereoKitC')

src_includes = include_directories('src')
thirdparty_includes = include_directories('thirdparty')
includes = [src_includes, thirdparty_includes]

src_files = []

subdir('src')
src_files += files('src/main.cpp')

executable(
	'stardustxr',
	src_files,
	include_directories: includes,
	link_args: ['-lflatbuffers', '-lX11', '-lGLX', '-lEGL', '-ldl', '-lopenxr_loader', '-lfontconfig'],
	dependencies: [
		libstardustxr,
		stereokit,
	],
	install : true,
)
