set(CMAKE_TOOLCHAIN_FILE ../libavrc++/libavrc++Toolchain.cmake)

project(libavrc++_examples)

libavrcpp_list_configs()

cmake_minimum_required(VERSION 2.6)

add_avr_firmware(fm_alarm_clock CONFIG arduino_pro_mini_3v PORT /dev/ttyUSB0
  fm_alarm_clock.cc
  )
