add_executable(sfml3.2 main.cpp)

#set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "/usr/local/lib/cmake/SFML")

find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

target_include_directories(sfml3.2 PRIVATE ${SFML_INCLUDE_DIR})
target_compile_features(sfml3.2 PUBLIC cxx_std_17)
target_compile_definitions(sfml3.2 PRIVATE SFML_STATIC)

target_link_libraries(sfml3.2 sfml-graphics sfml-window sfml-system)