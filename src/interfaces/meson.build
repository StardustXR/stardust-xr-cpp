interfaces_includes = include_directories('.')

lib_interfaces = static_library(
	'interfaces',
	files(
		'environment.cpp',
		'field.cpp',
		'input.cpp',
		'lifecycle.cpp',
		'model.cpp',
		'spatial.cpp',
	),
        include_directories: [interfaces_includes],
        dependencies: [stereokit]
)

interfaces = declare_dependency(
	include_directories: interfaces_includes,
	link_with: lib_interfaces
)
