# Generated by ard-make version 1.1

ARDMK_DIR = $(HOME)/Arduino-Makefile
ARDUINO_DIR = $(HOME)/arduino
#BOARDS_TXT = $(HOME)/arduino/hardware/arduino/avr/boards.txt
ARDMK_VENDOR = arduino
#BOARD_TAG = uno
#BOARD_SUB = atmega328
#TARGET = Uberprachtige-KBS-game

#CXXFLAGS += -Isrc/spel/
USER_LIB_PATH = $(realpath ./src)
#ARDUINO_LIBS = Adafruit_GFX_Library SPI Wire Adafruit_ILI9341 SD game level

include $(ARDMK_DIR)/Arduino.mk
