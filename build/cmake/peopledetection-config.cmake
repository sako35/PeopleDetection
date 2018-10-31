# PeopleDetection CMake config file
#
# This file sets the following variables:
# PeopleDetection_FOUND - Always TRUE.
# PeopleDetection_INCLUDE_DIRS - Directories containing the PeopleDetection include files.
# PeopleDetection_IDL_DIRS - Directories containing the PeopleDetection IDL files.
# PeopleDetection_LIBRARIES - Libraries needed to use PeopleDetection.
# PeopleDetection_DEFINITIONS - Compiler flags for PeopleDetection.
# PeopleDetection_VERSION - The version of PeopleDetection found.
# PeopleDetection_VERSION_MAJOR - The major version of PeopleDetection found.
# PeopleDetection_VERSION_MINOR - The minor version of PeopleDetection found.
# PeopleDetection_VERSION_REVISION - The revision version of PeopleDetection found.
# PeopleDetection_VERSION_CANDIDATE - The candidate version of PeopleDetection found.

message(STATUS "Found PeopleDetection-1.0.0")
set(PeopleDetection_FOUND TRUE)

find_package(<dependency> REQUIRED)

#set(PeopleDetection_INCLUDE_DIRS
#    "C:/Program Files (x86)/PeopleDetection/include/peopledetection-1"
#    ${<dependency>_INCLUDE_DIRS}
#    )
#
#set(PeopleDetection_IDL_DIRS
#    "C:/Program Files (x86)/PeopleDetection/include/peopledetection-1/idl")
set(PeopleDetection_INCLUDE_DIRS
    "C:/Program Files (x86)/PeopleDetection/include/"
    ${<dependency>_INCLUDE_DIRS}
    )
set(PeopleDetection_IDL_DIRS
    "C:/Program Files (x86)/PeopleDetection/include//idl")


if(WIN32)
    set(PeopleDetection_LIBRARIES
        "C:/Program Files (x86)/PeopleDetection/components/lib/peopledetection.lib"
        ${<dependency>_LIBRARIES}
        )
else(WIN32)
    set(PeopleDetection_LIBRARIES
        "C:/Program Files (x86)/PeopleDetection/components/lib/peopledetection.dll"
        ${<dependency>_LIBRARIES}
        )
endif(WIN32)

set(PeopleDetection_DEFINITIONS ${<dependency>_DEFINITIONS})

set(PeopleDetection_VERSION 1.0.0)
set(PeopleDetection_VERSION_MAJOR 1)
set(PeopleDetection_VERSION_MINOR 0)
set(PeopleDetection_VERSION_REVISION 0)
set(PeopleDetection_VERSION_CANDIDATE )

