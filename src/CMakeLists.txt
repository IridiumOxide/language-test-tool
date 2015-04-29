file(GLOB_RECURSE TESTYJEZ_SOURCES ./*.cpp)
file(GLOB_RECURSE TESTYJEZ_HEADERS ./*.h)

add_executable(testyJez ${TESTYJEZ_SOURCES} ${TESTYJEZ_HEADERS})
target_link_libraries(testyJez Qt5::Core Qt5::Widgets)

install(TARGETS testyJez RUNTIME DESTINATION bin)
