
macro(group name)
	set(project_group ${name})
endmacro()

macro(addProject name)
	add_executable(${name} WIN32 ${BASE} ${ARGN})
	if (NOT ${project_group} STREQUAL "")
        set_target_properties(${name} PROPERTIES FOLDER ${project_group})
    endif()
endmacro()

