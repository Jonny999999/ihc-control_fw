set(APP_NAME "hello")

add_executable(
    ${APP_NAME}
    main.cpp
  )

register_avr_upload_target(${APP_NAME})
