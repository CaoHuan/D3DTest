macro(addProject name)
	add_executable(${name} WIN32 ${ARGN})
	set(projects ${projects} ${name})
endmacro()
