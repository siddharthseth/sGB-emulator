project(sGB)
add_executable(sGB src/main.cpp src/CPU.cpp src/sGBEmulator.cpp src/clock.cpp src/MMU.cpp)
target_link_libraries(sGB ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
install(TARGETS sGB RUNTIME DESTINATION ${BIN_DIR})