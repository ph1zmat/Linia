# 2023/07/04 15:47:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Linia2)

#set(CMAKE_UNITY_BUILD OFF)

# Определение архитектуры системы для правильного поиска библиотек
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(ARCH_LIBDIR "x86_64-linux-gnu")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ARCH_LIBDIR "aarch64-linux-gnu")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7|arm")
    set(ARCH_LIBDIR "arm-linux-gnueabihf")
else()
    # Попробуем автоматически определить через pkg-config
    set(ARCH_LIBDIR "")
endif()

if(WIN32)

    include_directories(
        src
        ../../ThirdParty/wxWidgets/include
        ../../ThirdParty/wxWidgets/generated/lib/vc_lib/mswud
        src/ThirdParty/libmpg123/src/libmpg123/
        src/ThirdParty/libmpg123/config/win/x64/
    )

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MT -std:c++20")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /STACK:10000000")

    add_compile_options(
        # Обязательные для MSVC
        /wd4265    # class has virtual functions, but its non-trivial destructor is not virtual
        /wd4514    # unreferenced inline function has been removed
        /wd4625    # copy constructor was implicitly defined as deleted
        /wd4626    # assignment operator was implicitly defined as deleted
        /wd4710    # function not inlined
        /wd4711    # function xxx selected for automatic inline expansion
        /wd4820    # 'N' bytes padding added after data member
        /wd5026    # move constructor was implicitly defined as deleted
        /wd5027    # move assignment operator was implicitly defined as deleted
        /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd5039    # pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception
    )

else()

    include_directories(
        ./src
        ../../ThirdParty/wxWidgets/generated/lib/wx/include/gtk3-unicode-static-3.2
        ../../ThirdParty/wxWidgets/include
    )

    link_directories(
        ../../ThirdParty/wxWidgets/generated/lib
    )

    # Флаги для GCC/Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wno-unknown-pragmas")

    add_definitions(-D__WXGTK__ -D__WXGTK3__ -D_FILE_OFFSET_BITS=64)

endif()

add_definitions(-DOPT_GENERIC)

file(GLOB RESOURCES                     resources/*.*)
file(GLOB DOC                           doc/*.*)
file(GLOB SRC                           src/*.*)
file(GLOB COMMUNICATOR                  src/Communicator/*.*)
file(GLOB COMMUNICATOR_COMPORT          src/Communicator/ComPort/*.*)
file(GLOB COMMUNICATOR_SPI              src/Communicator/SPI/*.*)
file(GLOB CONTROLS                      src/Controls/*.*)
file(GLOB PANELS                        src/Panels/*.*)
file(GLOB PANELS_PANEL5                 src/Panels/Panel05_Graph/*.*)
file(GLOB PANELS_PANEL11                src/Panels/Panel11_Menu/*.*)
file(GLOB PANELS_PANEL12                src/Panels/Panel12_Debug/*.*)
file(GLOB SETTINGS                      src/Settings/*.*)
file(GLOB UTILS                         src/Utils/*.*)
file(GLOB WINDOWS                       src/Windows/*.*)

source_group(doc                        FILES ${DOC})
source_group(Resources                  FILES ${RESOURCES})
source_group(src                        FILES ${SRC})
source_group(Communicator               FILES ${COMMUNICATOR})
source_group(Communicator/ComPort       FILES ${COMMUNICATOR_COMPORT})
source_group(Communicator/SPI           FILES ${COMMUNICATOR_SPI})
source_group(Controls                   FILES ${CONTROLS})
source_group(Panels                     FILES ${PANELS})
source_group(Panels/Panel05_Graph       FILES ${PANELS_PANEL5})
source_group(Panels/Panel11_Menu        FILES ${PANELS_PANEL11})
source_group(Panels/Panel12_Debug       FILES ${PANELS_PANEL12})
source_group(Settings                   FILES ${SETTINGS})
source_group(Utils                      FILES ${UTILS})
source_group(Windows                    FILES ${WINDOWS})

add_executable(${PROJECT_NAME}
    ${DOC}
    ${RESOURCES}
    ${SRC}
    ${COMMUNICATOR}
    ${COMMUNICATOR_COMPORT}
    ${COMMUNICATOR_SPI}
    ${CONTROLS}
    ${PANELS}
    ${PANELS_PANEL5}
    ${PANELS_PANEL11}
    ${PANELS_PANEL12}
    ${SETTINGS}
    ${UTILS}
    ${WINDOWS}
)


if(WIN32)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxbase32u)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxbase32u_net)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxmsw32u_core)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxmsw32u_aui)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxzlib)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxregexu)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxpng)

    target_link_libraries(${PROJECT_NAME} WinMM.lib)
    target_link_libraries(${PROJECT_NAME} Ws2_32.lib)
    target_link_libraries(${PROJECT_NAME} version.lib)
    target_link_libraries(${PROJECT_NAME} Imm32.Lib)
    target_link_libraries(${PROJECT_NAME} Setupapi.lib)
    target_link_libraries(${PROJECT_NAME} Comctl32.lib)
    target_link_libraries(${PROJECT_NAME} Rpcrt4.lib)

    # Копируем в рабочий каталог
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy $(OutputPath)\\$(PROJECT_NAME) . /Y)
else()
    # Найти системные библиотеки автоматически для любой архитектуры
    find_library(X11_LIBRARY X11)
    find_library(XKBCOMMON_LIBRARY xkbcommon)
    find_library(PNG_LIBRARY png)
    find_library(GSPELL_LIBRARY gspell-1)
    
    # Если автоматический поиск не сработал, попробуем по архитектуре
    if(NOT X11_LIBRARY AND ARCH_LIBDIR)
        find_library(X11_LIBRARY X11 PATHS /usr/lib/${ARCH_LIBDIR} NO_DEFAULT_PATH)
    endif()
    if(NOT XKBCOMMON_LIBRARY AND ARCH_LIBDIR)
        find_library(XKBCOMMON_LIBRARY xkbcommon PATHS /usr/lib/${ARCH_LIBDIR} NO_DEFAULT_PATH)
    endif()
    if(NOT PNG_LIBRARY AND ARCH_LIBDIR)
        find_library(PNG_LIBRARY png PATHS /usr/lib/${ARCH_LIBDIR} NO_DEFAULT_PATH)
    endif()
    if(NOT GSPELL_LIBRARY AND ARCH_LIBDIR)
        find_library(GSPELL_LIBRARY gspell-1 PATHS /usr/lib/${ARCH_LIBDIR} NO_DEFAULT_PATH)
    endif()
    
    # Вывод информации для отладки
    message(STATUS "System architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    message(STATUS "Library directory: ${ARCH_LIBDIR}")
    message(STATUS "X11 library: ${X11_LIBRARY}")
    message(STATUS "xkbcommon library: ${XKBCOMMON_LIBRARY}")
    message(STATUS "PNG library: ${PNG_LIBRARY}")
    message(STATUS "GSpell library: ${GSPELL_LIBRARY}")
    
    target_link_libraries(${PROJECT_NAME}
        wx_gtk3u_core-3.2
        gtk-3
        gdk-3
        pangocairo-1.0
        pango-1.0
        pthread
        cairo
        ${X11_LIBRARY}
        ${XKBCOMMON_LIBRARY}
        ${PNG_LIBRARY}
        gdk_pixbuf-2.0
        wx_baseu-3.2
        gio-2.0
        gobject-2.0
        glib-2.0
        ${GSPELL_LIBRARY}
        dl 
        )
endif()
