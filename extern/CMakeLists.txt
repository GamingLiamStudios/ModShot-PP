include(FetchContent)

# Include zlib-ng
FetchContent_Declare(
  zlib_ng
  GIT_REPOSITORY https://github.com/zlib-ng/zlib-ng
  GIT_TAG        6539b769e677bc1e6f4954d4457aba0cd138468b
)
FetchContent_GetProperties(zlib_ng)
if(NOT zlib_ng_POPULATED)
  FetchContent_Populate(zlib_ng)

  # Set custom variables, policies, etc.
  set(ZLIB_ENABLE_TESTS OFF)
  set(WITH_NATIVE_INSTRUCTIONS ON)

  add_subdirectory(${zlib_ng_SOURCE_DIR} ${zlib_ng_BINARY_DIR})
endif()

FetchContent_Declare(
  boxer
  GIT_REPOSITORY https://github.com/aaronmjacobs/Boxer
  GIT_TAG 01c17f6a9cd66068f7890ea887ab3b9a673f0434
)
FetchContent_GetProperties(boxer)
if(NOT boxer_POPULATED)
  FetchContent_Populate(boxer)

  # Set custom variables, policies, etc.

  add_subdirectory(${boxer_SOURCE_DIR} ${boxer_BINARY_DIR})
endif()
