cmake_minimum_required(VERSION 3.4)
project(BerserkBalls)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE( OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer osgText)

INCLUDE_DIRECTORIES( ${OPENSCENEGRAPH_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 1)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

SET(MYSOURCE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindowform.ui
    osgwidget.h
    osgwidget.cpp
    thespherecreator.cpp
    thespherecreator.h
    )

IF (NOT TARGET gtest)
    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory(googletest)
ENDIF()

add_library(physicsVector STATIC
    createvector.cpp
    createvector.h
    itsphysics.cpp
    itsphysics.h
    )
add_library(objectPosition STATIC
    objectposition.cpp
    objectposition.h
    )

add_executable(myTests
    physicsUnitTests.cpp
)
target_link_libraries(myTests physicsVector objectPosition gtest gtest_main)

add_executable(${PROJECT_NAME}
    ${MYSOURCE}
    )
target_link_libraries(${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}	
    Qt5::Widgets
    Qt5::Gui
    objectPosition
    physicsVector
)
