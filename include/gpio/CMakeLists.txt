project("gpio tools")

add_library(
    gpio
    gpio_evaluateSwitch.cpp
    gpio_output.cpp
    a2d.c
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
