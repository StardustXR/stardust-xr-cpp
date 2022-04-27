project('stardustxr', 'cpp', default_options: ['cpp_std=c++17'], version:'0.55.0')

add_project_arguments([
        '-DWLR_USE_UNSTABLE',
#	'-Dwlroots:gles2-renderer=enabled',
], language: 'cpp')

cmake = import('cmake')
pkg = import('pkgconfig')
libstardustxr = dependency('stardustxr-server', required: true)
flatbuffers = dependency('flatbuffers', version:'>=2.0.6')

stereokit_options = cmake.subproject_options()
stereokit_options.add_cmake_defines({'CMAKE_BUILD_TYPE': get_option('buildtype')})
stereokit_options.add_cmake_defines({'SK_LINUX_EGL': true, 'SK_BUILD_TESTS': false, 'SK_BUILD_SHARED_LIBS': false, 'SK_PHYSICS': false})
stereokit_options.add_cmake_defines({'CMAKE_CXX_FLAGS': '-Wno-unused-variable -Wno-unused-but-set-variable -Wno-unknown-pragmas -Wno-sign-compare -Wno-switch -Wno-parentheses -Wno-sizeof-pointer-memaccess'})
stereokit_options.set_install(false)
stereokit_project = cmake.subproject('StereoKit', options: stereokit_options)
stereokit = stereokit_project.dependency('StereoKitC')

xdg_utils_project = cmake.subproject('xdg-utils')
xdg_utils_basedir = xdg_utils_project.dependency('BaseDir')

wlroots_proj = subproject('wlroots', default_options: ['default_library=static', 'backends=[]', 'renderers=[\'gles2\']', 'examples=false', 'xcb-errors=disabled'])
wlroots = wlroots_proj.get_variable('wlroots')
wlroots_server_protocols = wlroots_proj.get_variable('protocols_server_header')
server_protocols = [
        wlroots_server_protocols['xdg-shell']
]

src_includes = include_directories('src')
thirdparty_includes = include_directories('thirdparty')
includes = [src_includes, thirdparty_includes]

src_files = []

subdir('src')
src_files += server_protocols
src_files += files('src/main.cpp')

executable(
	'stardustxr',
	src_files,
	include_directories: includes,
        link_args: ['-lX11', '-lGLX', '-lEGL', '-ldl', '-lopenxr_loader', '-lfontconfig'],
        # link_args: ['-lX11', '-lGLX', '-lGLEW', '-lGL', '-ldl', '-lopenxr_loader', '-lfontconfig'],
	dependencies: [
		libstardustxr,
                flatbuffers,
                stereokit,
                wlroots,
                xdg_utils_basedir,
	],
	install : true,
)
