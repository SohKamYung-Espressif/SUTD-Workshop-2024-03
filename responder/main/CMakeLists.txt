idf_component_register(SRC_DIRS "."
                       INCLUDE_DIRS ".")

set_source_files_properties("app_driver.c" PROPERTIES COMPILE_FLAGS -Wno-format)