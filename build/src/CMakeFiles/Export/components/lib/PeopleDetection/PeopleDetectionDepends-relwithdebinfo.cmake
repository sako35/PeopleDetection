#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PeopleDetection" for configuration "RelWithDebInfo"
set_property(TARGET PeopleDetection APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(PeopleDetection PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/components/lib/PeopleDetection.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELWITHDEBINFO "RTC112_vc12;coil112_vc12;omniORB421_rt;omniDynamic421_rt;omnithread40_rt;advapi32;ws2_32;mswsock;opencv_videostab;opencv_video;opencv_ts;opencv_superres;opencv_stitching;opencv_photo;opencv_ocl;opencv_objdetect;opencv_nonfree;opencv_ml;opencv_legacy;opencv_imgproc;opencv_highgui;opencv_gpu;opencv_flann;opencv_features2d;opencv_core;opencv_contrib;opencv_calib3d;Kinect20"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/components/bin/PeopleDetection.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS PeopleDetection )
list(APPEND _IMPORT_CHECK_FILES_FOR_PeopleDetection "${_IMPORT_PREFIX}/components/lib/PeopleDetection.lib" "${_IMPORT_PREFIX}/components/bin/PeopleDetection.dll" )

# Import target "PeopleDetectionComp" for configuration "RelWithDebInfo"
set_property(TARGET PeopleDetectionComp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(PeopleDetectionComp PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/components/bin/PeopleDetectionComp.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS PeopleDetectionComp )
list(APPEND _IMPORT_CHECK_FILES_FOR_PeopleDetectionComp "${_IMPORT_PREFIX}/components/bin/PeopleDetectionComp.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
