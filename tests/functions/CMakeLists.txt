add_executable(range src/range.cpp)
add_executable(trigonometric src/trigonometric.cpp)
add_executable(reduce src/reduce.cpp)
add_executable(argmax src/argmax.cpp)

add_test(range range)
add_test(trigonometric trigonometric)
add_test(reduce reduce)
add_test(argmax argmax)
