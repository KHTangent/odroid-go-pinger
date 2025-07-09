zephyr_library()

AUX_SOURCE_DIRECTORY(src/ui UIFILES)

zephyr_library_sources(${UIFILES} src/ui_wrapper.c)

