set(CMAKE_CXX_STANDARD 14)
# Remove Wno-unused function once all zmq  functions we need are used
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O0 -Wextra")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-deprecated-declarations -Wno-unused-function -Wno-unused-parameter")
