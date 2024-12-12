# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "frontend\\CMakeFiles\\frontend_autogen.dir\\AutogenUsed.txt"
  "frontend\\CMakeFiles\\frontend_autogen.dir\\ParseCache.txt"
  "frontend\\frontend_autogen"
  )
endif()
