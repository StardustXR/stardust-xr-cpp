core_includes = include_directories('.')

lib_core = static_library(
	'core',
	files(
		'client.cpp',
		'clientmanager.cpp',
		'scenegraph.cpp',
	),
	include_directories: [core_includes],
        dependencies: [libstardustxr]
)

core = declare_dependency(
	include_directories: core_includes,
	link_with: lib_core
)
