project("time")

add_library(
    time
    time.c
    clock.cpp
    pulse.cpp
    )

target_include_directories(
  time
  PUBLIC 
  .
  )
