set(APP_NAME "hello")

add_executable(
    ${APP_NAME}
    main.cpp
  )

target_link_libraries(
    hello
    time
    gpio
    )


register_avr_upload_target(${APP_NAME})
