project("time")

add_library(
    time
    time.c
    clock.cpp
    )

target_include_directories(
  time
  PUBLIC 
  .
  )
