idf_component_register(
    SRCS
        "src/m5stdio.cpp"
        "src/m5util.cpp"
        "src/m5i2c.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        M5Unified
)
target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)
