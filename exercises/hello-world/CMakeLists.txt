if(WIN32)
  # ASM_NASM support for VS 2010 and above was introduced in version 3.9
  cmake_minimum_required(VERSION 3.9)
else()
  # The COMPILE_LANGUAGE generator expression was introduced in version 3.3
  cmake_minimum_required(VERSION 3.3)
endif()

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

project(${exercise} CXX ASM_NASM)

# Get a source filename from the exercise name by replacing -'s with _'s
string(REPLACE "-" "_" file ${exercise})

add_executable(${exercise} main.cpp ${file}.asm ${file}_test.cpp)

target_compile_options(${exercise} PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM_NASM>>:-g>
  $<$<COMPILE_LANGUAGE:ASM_NASM>:-Werror>
)

if("${CMAKE_ASM_NASM_OBJECT_FORMAT}" STREQUAL "elf")
  # Use DWARF debug information format
  target_compile_options(${exercise} PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM_NASM>>:-F dwarf>
  )
else()
  # Prepend underscore to all global and external variables, as C requires it.
  # For ELF, though, the leading underscore should not be used.
  target_compile_options(${exercise} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM_NASM>:--gprefix _>
  )
endif()

# C++11 is required by Catch2
set_target_properties(${exercise} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
  target_compile_options(${exercise} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall>
    $<$<COMPILE_LANGUAGE:CXX>:-Wextra>
    $<$<COMPILE_LANGUAGE:CXX>:-pedantic>
    $<$<COMPILE_LANGUAGE:CXX>:-Werror>
    # Generate code for 32-bit ABI
    $<$<COMPILE_LANGUAGE:CXX>:-m32>
  )
  set_target_properties(${exercise} PROPERTIES LINK_FLAGS -m32)
endif()

# Run the tests on every build
add_custom_target(${exercise}-test ALL DEPENDS ${exercise} COMMAND ${exercise})
