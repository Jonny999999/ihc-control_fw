project("gpio tools")

add_library(
    gpio
    gpio_evaluateSwitch.cpp
    gpio_output.cpp
    )

target_link_libraries(
  gpio
  time
  )

target_include_directories(
  gpio
  PUBLIC 
  .
  )
