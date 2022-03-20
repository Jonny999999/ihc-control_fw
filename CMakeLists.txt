cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/avr.toolchain.cmake")

project(TestAvrProject)

# export compile commands to json file (so that ycm and IDEs can complete)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# get MessagePack en-/de-coder:
include(FetchContent)
FetchContent_Declare(
  zcmessagepack
  GIT_REPOSITORY https://github.com/rainerschoe/ZeroCopyMessagePack.git
  GIT_TAG        283e283a312f4eb49858e033c96f6d684d26e27d
)
FetchContent_GetProperties(zcmessagepack)
FetchContent_Populate(zcmessagepack)
# Only populate here, as we need to add our own targets plattform specific (in avr.cmake)
# Cannot use provided cmake files.

add_subdirectory(application)
