set(AVRDUDE ${CMAKE_CURRENT_LIST_DIR}/avrdude_extension.sh)
set(AVRDUDE_PORT /dev/ttyUSB0)
set(AVRDUDE_PROGRAMMER usbasp)



# works for Atmega328pb and Atmega1284p
# Bit 2 BODLEVEL=101 => Min. V BOT=2.5
set(FUSE_EXTENDED 0b11111101:m)

# Bit 5 BLB1x=00 => SPM is not allowed to write to the Boot Loader section, and LPM executing
# TODO Does not work 
set(LOCK_BYTE 0b11001111:m)

# Patch for fixing supporting atmega328pb
set(AVRDUDE_FLAGS -p ${BOARD_MCU} -c ${AVRDUDE_PROGRAMMER})

# fuses... example for atmega328pb:
# Need to change for different MCU
#    # Atmega328pb:
#    # Bit 3 EESAVE=0 => EEPROM memory is preserved through the chip erase
#    # Bit 2 BOOTSZ=00 => boot loader section (maximum size) in nrww und application in rww
#    # Bit 2 BOOTSZ=00 => boot loader section (2*4096 bytes) in nrww und application in rww
#    # Bit 0 BOOTRST=0 => jump to bootloader after reset
#    # disable jtag (atmega1284 only)
#    set(FUSE_HIGH 0b11010000:m)
#
#    # configure fuses for ATMEGA1284p 16MHZ crystal low power:
#    set(FUSE_LOW 0b11111111:m)
#
#    set(AVRDUDE_WRITE_BYTES -U efuse:w:${FUSE_EXTENDED}  -U lfuse:w:${FUSE_LOW} -U hfuse:w:${FUSE_HIGH})

