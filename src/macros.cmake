macro(addProject name)
	add_executable(${name} WIN32 ${BASE} ${ARGN})
	set(projects ${projects} ${name})
endmacro()
