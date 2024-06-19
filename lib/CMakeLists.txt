
include(FetchContent)

set(EXTERNAL_DEPENDENCIES 
    "fmt" 
    "fastgltf"
    # "glfw"
)
set(EXTERNAL_DEPENDENCIES_REPO 
    "https://github.com/fmtlib/fmt"  
    "https://github.com/spnda/fastgltf"
    "https://github.com/glfw/glfw"
)
set(EXTERNAL_DEPENDENCIES_TAG 
    "master"
    "main"
    "master"
)

list(LENGTH EXTERNAL_DEPENDENCIES EXTERNAL_DEPENDENCIES_COUNT)

math(EXPR EXTERNAL_DEPENDENCIES_COUNT "${EXTERNAL_DEPENDENCIES_COUNT} - 1")

foreach(index RANGE 0 ${EXTERNAL_DEPENDENCIES_COUNT})
    list(GET EXTERNAL_DEPENDENCIES ${index} DEPENDENCY)
    list(GET EXTERNAL_DEPENDENCIES_REPO ${index} GIT_REPOSITORY)
    list(GET EXTERNAL_DEPENDENCIES_TAG ${index} GIT_TAG)

    message(STATUS "Fetching ${DEPENDENCY}")
    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DEPENDENCY})
        FetchContent_Declare(
            fetch_${DEPENDENCY}
            GIT_REPOSITORY ${GIT_REPOSITORY}
            GIT_TAG ${GIT_TAG}
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${DEPENDENCY}
        )
        FetchContent_MakeAvailable(fetch_${DEPENDENCY})
    else()
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${DEPENDENCY})
    endif()

endforeach()

# add_subdirectory(glfw/src)
add_subdirectory(ecs)







