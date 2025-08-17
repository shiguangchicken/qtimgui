# imgui library: imgui is build by default, but you can
# provide your own version by setting QTIMGUI_BUILD_IMGUI to OFF
option(QTIMGUI_BUILD_IMGUI "Use imgui provided as qtimgui submodule" ON)
if (QTIMGUI_BUILD_IMGUI)
    set(IMGUI_SOURCES
      imgui/imgui.cpp
      imgui/imgui_demo.cpp
      imgui/imgui_draw.cpp
      imgui/imgui_widgets.cpp
      imgui/imgui_tables.cpp
    )

    set(IMGUI_HEADERS
      imgui/imconfig.h
      imgui/imgui.h
      imgui/imgui_internal.h
      imgui/imstb_rectpack.h
      imgui/imstb_textedit.h
      imgui/imstb_truetype.h
    )

    add_library(imgui
      STATIC
      ${IMGUI_SOURCES}
    )
    target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
    install(TARGETS imgui ARCHIVE DESTINATION qtimgui/lib)
    install(FILES ${IMGUI_HEADERS} DESTINATION qtimgui/include)
endif(QTIMGUI_BUILD_IMGUI)

# implot library: implot is build by default, but you can
# provide your own version by setting QTIMGUI_BUILD_IMPLOT to OFF
option(QTIMGUI_BUILD_IMPLOT "Use implot provided as qtimgui submodule" ON)
if (QTIMGUI_BUILD_IMPLOT)
    set(IMPLOT_SOURCES
      implot/implot.cpp
      implot/implot_items.cpp
      implot/implot_demo.cpp
    )

    set(IMPLOT_HEADERS
      implot/implot.h
      implot/implot_internal.h
    )

    add_library(implot
      STATIC
      ${IMPLOT_SOURCES}
    )
    target_include_directories(implot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
    target_include_directories(implot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/implot)
    target_link_libraries(implot PRIVATE imgui)
    install(TARGETS implot ARCHIVE DESTINATION qtimgui/lib)
    install(FILES ${IMPLOT_HEADERS} DESTINATION qtimgui/include)
endif(QTIMGUI_BUILD_IMPLOT)
