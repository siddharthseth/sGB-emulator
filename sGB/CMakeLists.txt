project(sGB)
add_executable(sGB src/main.cpp)
target_link_libraries(sGB ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
install(TARGETS sGB RUNTIME DESTINATION ${BIN_DIR})