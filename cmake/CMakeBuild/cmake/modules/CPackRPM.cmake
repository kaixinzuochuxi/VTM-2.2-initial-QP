#if( CMAKE_VERSION VERSION_LESS 3.11 )
  message( "Loading patched CPackRPM.cmake to get component specific versioning." )
  include( ${CMAKE_CURRENT_LIST_DIR}/patched/CPackRPM.cmake )
#else()
#  include( ${CMAKE_ROOT}/Modules/CPackDeb.cmake )
#endif()
