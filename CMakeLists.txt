zephyr_syscall_include_directories(include)

zephyr_include_directories(include)
zephyr_include_directories(gui/src/ui)

add_subdirectory(drivers)
add_subdirectory(lib)
add_subdirectory(gui)
