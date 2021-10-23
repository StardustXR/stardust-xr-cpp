project('stardustxr', 'cpp', default_options: ['cpp_std=c++17'], version:'0.55.0')

cmake = import('cmake')
pkg = import('pkgconfig')
libstardustxr = dependency('stardustxr-server', required: true)

stereokit_options = cmake.subproject_options()
stereokit_options.add_cmake_defines({'SK_LINUX_EGL': true, 'SK_BUILD_TESTS': false, 'SK_BUILD_SHARED_LIBS': false})
# stereokit_options.add_cmake_defines({'SK_BUILD_TESTS': false, 'SK_BUILD_SHARED_LIBS': false})
stereokit_options.add_cmake_defines({'CMAKE_CXX_FLAGS': '-Wno-unused-variable -Wno-unused-but-set-variable -Wno-unknown-pragmas -Wno-sign-compare -Wno-switch -Wno-parentheses -Wno-sizeof-pointer-memaccess'})
stereokit_options.set_install(false)
stereokit_project = cmake.subproject('StereoKit', options: stereokit_options)
stereokit = stereokit_project.dependency('StereoKitC')
reactphysics = stereokit_project.dependency('reactphysics3d')

xdg_utils_project = cmake.subproject('xdg-utils')
xdg_utils_basedir = xdg_utils_project.dependency('BaseDir')

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
	# link_args: ['-lflatbuffers', '-lX11', '-lGLX', '-lGLEW', '-lGL', '-ldl', '-lopenxr_loader', '-lfontconfig'],
	dependencies: [
		libstardustxr,
		stereokit,
		reactphysics,
                xdg_utils_basedir,
	],
	install : true,
)
