IF (MSVC)

    SET(SDL_NAME SDL)
    SET(ZLIB_NAME zdll)

ELSEIF (MINGW)

    SET(SDL_NAME SDL)
    SET(ZLIB_NAME z)

ELSEIF (APPLE)

    FIND_PACKAGE(SDL REQUIRED)
    SET(SDL_NAME ${SDL_LIBRARY})
    SET(ZLIB_NAME z)

ELSEIF (UNIX)

    SET(SDL_NAME SDL)
    SET(ZLIB_NAME z)

ENDIF ()

SET(LIBTCOD_C_SRC bresenham_c.c bsp_c.c color_c.c console_c.c fov_c.c fov_circular_raycasting.c
        fov_diamond_raycasting.c fov_permissive.c fov_permissive2.c fov_recursive_shadowcasting.c
        fov_restrictive.c heightmap_c.c image_c.c lex_c.c list_c.c mersenne_c.c namegen_c.c
        noise_c.c parser_c.c path_c.c sys_c.c sys_opengl_c.c sys_sdl_c.c sys_sdl_img_bmp.c
        sys_sdl_img_png.c tree_c.c txtfield_c.c wrappers.c zip_c.c png/lodepng.c)

SET(LIBTCOD_CPP_SRC bresenham.cpp bsp.cpp color.cpp console.cpp fov.cpp heightmap.cpp image.cpp lex.cpp mersenne.cpp
        mouse.cpp namegen.cpp noise.cpp parser.cpp path.cpp sys.cpp txtfield.cpp zip.cpp)

SET(LIBTCOD_GUI_SRC gui/button.cpp gui/container.cpp gui/flatlist.cpp gui/hbox.cpp gui/image.cpp gui/label.cpp gui/radiobutton.cpp
        gui/slider.cpp gui/statusbar.cpp gui/textbox.cpp gui/togglebutton.cpp gui/toolbar.cpp gui/vbox.cpp gui/widget.cpp)

# Linux wants seperate C and C++ libraries.
IF (UNIX AND NOT APPLE)
    INCLUDE_DIRECTORIES(${EXTERNAL_INCLUDES})
    ADD_LIBRARY(${LIBTCOD_NAME} SHARED ${LIBTCOD_C_SRC})
    TARGET_LINK_LIBRARIES(${LIBTCOD_NAME} ${EXTERNAL_LIBS})

    ADD_LIBRARY(${LIBTCOD_CPP_NAME} SHARED ${LIBTCOD_CPP_SRC})
    TARGET_LINK_LIBRARIES(${LIBTCOD_CPP_NAME} ${LIBTCOD_NAME} ${EXTERNAL_LIBS})
ELSE ()
    ADD_LIBRARY(${LIBTCOD_NAME} SHARED ${LIBTCOD_C_SRC} ${LIBTCOD_CPP_SRC})
    TARGET_LINK_LIBRARIES(${LIBTCOD_NAME} ${SDL_NAME} ${ZLIB_NAME} ${OPENGL_LIBRARY})
ENDIF ()

INCLUDE_DIRECTORIES(${GUI_INCLUDE_DIR})
ADD_LIBRARY(${LIBTCOD_GUI_NAME} SHARED ${LIBTCOD_GUI_SRC})
TARGET_LINK_LIBRARIES(${LIBTCOD_GUI_NAME} ${LIBTCOD_NAME} ${LIBTCOD_CPP_NAME})  #${LIBTCOD_CPP_NAME} is empty on non-linux platforms

IF (MSVC OR MINGW)
    # Since we're exporting functions and clases, we use the "standard" macro combinations to do so under msvc.
    # These turn them on for the right builds.
    SET_PROPERTY(TARGET ${LIBTCOD_NAME} PROPERTY COMPILE_DEFINITIONS LIBTCOD_EXPORTS)
    SET_PROPERTY(TARGET ${LIBTCOD_GUI_NAME} PROPERTY COMPILE_DEFINITIONS LIBTCOD_GUI_EXPORTS)
ENDIF ()