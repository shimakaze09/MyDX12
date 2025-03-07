GET_TARGET_NAME(COMMON "${PROJECT_SOURCE_DIR}/src/test/common")
ADD_TARGET(
        TEST
        MODE EXE
        LIB ${COMMON}
)
