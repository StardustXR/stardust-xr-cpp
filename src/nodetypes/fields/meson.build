fields_nodetypes_includes = include_directories('.')

lib_fields_nodetypes = static_library(
	'fields_nodetypes',
	files(
		'boxfield.cpp',
		'field.cpp',
	),
	include_directories: [fields_nodetypes_includes]
)

fields_nodetypes = declare_dependency(
	include_directories: fields_nodetypes_includes,
	link_with: lib_fields_nodetypes
)
