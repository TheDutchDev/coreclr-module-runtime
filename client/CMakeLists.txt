cmake_minimum_required (VERSION 3.8)

set(TARGET_NAME altv-client-csharp)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CSHARP_MODULE_DEFS
    # Compliation
    -DCXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
    -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

    # Platform
    -DUNICODE
    -D_UNICODE

    # alt:V
    -DALT_CLIENT
    -DALT_CLIENT_API

    # Debug
#   -DDEBUG_CLIENT
)

# cpp-sdk
if(NOT ALTV_CSHARP_CPP_SDK)
  set(ALTV_CSHARP_CPP_SDK ../)
else()
  set(ALTV_CSHARP_DEINIT_CPPSDK 1)
endif()

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.cpp")
file(GLOB_RECURSE CAPI_FILES "../c-api/*.h" "../c-api/*.cpp")

if(MSVC AND NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_definitions(/MP)
endif()

set(SDK_PROJECT_NAME alt-sdk-csharp)
if(NOT TARGET ${SDK_PROJECT_NAME})
    include(${ALTV_CSHARP_CPP_SDK}/cpp-sdk/CMakeLists.txt)
endif()

if(DYNAMIC_BUILD)
    add_library(${TARGET_NAME} SHARED "${PROJECT_SOURCE_FILES}" "${CAPI_FILES}")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        ${CSHARP_MODULE_DEFS}
        -DALTV_CSHARP_SHARED
    )
    include_directories(${TARGET_NAME} PRIVATE "src" "thirdparty" "..")
    add_dependencies(${TARGET_NAME} alt-sdk-csharp)
else()
    add_library(${TARGET_NAME}-static STATIC "${PROJECT_SOURCE_FILES}" "${CAPI_FILES}")
    target_compile_definitions(${PROJECT_NAME}-static PRIVATE
        ${CSHARP_MODULE_DEFS}
    )
    include_directories(${TARGET_NAME}-static PRIVATE "src" "thirdparty" ${ALTV_CSHARP_CPP_SDK})
endif()


if(ALTV_CSHARP_DEINIT_CPPSDK)
  if(CMAKE_HOST_WIN32)
      add_custom_command(TARGET ${PROJECT_NAME}-static
        PRE_BUILD
        COMMAND cmd /C "tools\\deinit-cppsdk.bat"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      )
  else()
      if(EXISTS "../cpp-sdk/SDK.h")
          add_custom_command(TARGET ${PROJECT_NAME}-static
            PRE_BUILD
            COMMAND "git submodule deinit -f ../cpp-sdk"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
          )
      endif()
  endif()
endif()
