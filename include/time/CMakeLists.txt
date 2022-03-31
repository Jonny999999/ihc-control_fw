project("time")

add_library(
    time
    time.c
    )

target_include_directories(
  time
  PUBLIC 
  .
  )
