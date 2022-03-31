project("evaluateSwitch")

add_library(
    evaluateSwitch
    evaluateSwitch.cpp
    )

target_link_libraries(
  evaluateSwitch
  time
  )

target_include_directories(
  evaluateSwitch
  PUBLIC 
  .
  )
