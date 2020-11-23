project('stardustxr', 'cpp', default_options : ['cpp_std=c++17'])

pkg = import('pkgconfig')
libstardustxr = dependency('stardustxr-server', required: true)
stereokit = dependency('StereoKitC', required: true)

includes = include_directories('src')
subdir('src')

executable(
	'stardustxr',
	'src/main.cpp',
	include_directories: includes,
	dependencies: [
		libstardustxr,
		stereokit,
		# After here are all the internal dependencies
		nodetypes,
		scenegraph,
		tests,
	]
)
