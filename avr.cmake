cmake_minimum_required(VERSION 3.15)

###############################################################################
### AVR controller settings:
###############################################################################
# Clock frequency in Hz:
set(F_CPU 1000000UL)
add_definitions(-DF_CPU=${F_CPU})

# Controller type:
set(BOARD_MCU "atmega8")
add_compile_options(-mmcu=${BOARD_MCU})
add_link_options(-mmcu=${BOARD_MCU})

###############################################################################
### AVRDUDE settings:
###############################################################################
# programmer board type:
set(AVRDUDE_PROGRAMMER usbasp)

set(AVRDUDE_FLAGS -p ${BOARD_MCU} -c ${AVRDUDE_PROGRAMMER})

# registers the upload target for the specified BIN_NAME 
# after calling this function you may upload/flash your application 
# by "building" the BIN_NAME_upload target
function(register_avr_upload_target BIN_NAME)
    #add_custom_target(${BIN_NAME}_elf2bin ALL ${CMAKE_OBJCOPY} -O binary -R .eeprom ${BIN_NAME} ${BIN_NAME}.bin
    #    DEPENDS  ${BIN_NAME}
    #    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    #    VERBATIM 
    #    )
    add_custom_target(${BIN_NAME}_elf2hex ALL ${CMAKE_OBJCOPY} -O ihex -R .eeprom ${BIN_NAME} ${BIN_NAME}.hex
        DEPENDS  ${BIN_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        VERBATIM 
        )
    set(AVRDUDE_WRITE_FLASH -U flash:w:${BIN_NAME}.hex)
    add_custom_target(${BIN_NAME}_upload ${AVRDUDE} ${AVRDUDE_FLAGS} -U flash:w:${BIN_NAME}.hex
        DEPENDS  ${BIN_NAME}_elf2hex
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        VERBATIM 
        )
endfunction()
