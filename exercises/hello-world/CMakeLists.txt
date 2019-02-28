# Set the minimum version required by the FetchContent module
cmake_minimum_required(VERSION 3.11)

# Get the exercise name from the current directory
get_filename_component(exercise ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Force 32-bit output file format to avoid different calling conventions
if(WIN32)
  set(CMAKE_ASM_NASM_OBJECT_FORMAT win32)
elseif(APPLE)
  set(CMAKE_ASM_NASM_OBJECT_FORMAT macho)
else()
  set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
endif()

project(${exercise} C ASM_NASM)

include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG v2.4.3
)

FetchContent_GetProperties(unity)
if(NOT unity_POPULATED)
  FetchContent_Populate(unity)
endif()

# Get a source filename from the exercise name by replacing -'s with _'s
string(REPLACE "-" "_" file ${exercise})

add_executable(${exercise}
  ${file}.asm
  ${file}_test.c
  ${unity_SOURCE_DIR}/src/unity.c
)

target_include_directories(${exercise} PRIVATE ${unity_SOURCE_DIR}/src)

target_compile_options(${exercise} PRIVATE
  $<$<COMPILE_LANGUAGE:ASM_NASM>:-g>
  $<$<COMPILE_LANGUAGE:ASM_NASM>:-Werror>
)

if("${CMAKE_ASM_NASM_OBJECT_FORMAT}" STREQUAL "elf")
  # Use DWARF debug information format
  target_compile_options(${exercise} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM_NASM>:-F dwarf>
  )
else()
  # Prepend underscore to all global and external variables, as C requires it.
  # For ELF, though, the leading underscore should not be used.
  target_compile_options(${exercise} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM_NASM>:--gprefix _>
  )
endif()

set_target_properties(${exercise} PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED OFF
  C_EXTENSIONS OFF
)

if("${CMAKE_C_COMPILER_ID}" MATCHES "(GNU|Clang)")
  target_compile_options(${exercise} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-g>
    $<$<COMPILE_LANGUAGE:C>:-Wall>
    $<$<COMPILE_LANGUAGE:C>:-Wextra>
    $<$<COMPILE_LANGUAGE:C>:-pedantic>
    $<$<COMPILE_LANGUAGE:C>:-Werror>
    # Generate code for 32-bit ABI
    $<$<COMPILE_LANGUAGE:C>:-m32>
  )
  set_target_properties(${exercise} PROPERTIES LINK_FLAGS -m32)
endif()

# Run the tests on every build
add_custom_target(${exercise}-test ALL DEPENDS ${exercise} COMMAND ${exercise})
