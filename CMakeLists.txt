project(graph)

add_executable(graph ostergard.cpp svert.cpp adjmat.cpp main.cpp)

install(TARGETS graph RUNTIME DESTINATION bin)
