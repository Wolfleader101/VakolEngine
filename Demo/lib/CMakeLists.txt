# Assuming your current directory structure is something like:
# Demo/
# |- lib/
# |- implot/
# |- imgui/

# Add implot library
add_library(implot
    implot/implot.cpp
    implot/implot_items.cpp
)

# Include directories for implot and its dependencies
target_include_directories(implot PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/implot" # For implot headers
    "${CMAKE_SOURCE_DIR}/Core/lib/imgui/imgui" # For imgui headers
)