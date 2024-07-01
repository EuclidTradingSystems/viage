# Adapted from https://github.com/christopro/AwesomeWorld-Cmake/blob/main/AwesomeWorld/deploy-qt-windows.cmake.in

set(MACEPLOYQT "/Users/arthurkeller/Qt/6.6.2/macos/bin/macdeployqt")
set(COMPONENT_NAME_MAIN "viage")
set(CMAKE_CURRENT_SOURCE_DIR "/Users/arthurkeller/Documents/repos/viage/client")
set(CMAKE_BINARY_DIR "/Users/arthurkeller/Documents/repos/build-viage-Qt_6_6_2_for_macOS-Release")

execute_process(COMMAND ${MACDEPLOYQT}
                ${CMAKE_BINARY_DIR}/bin/${COMPONENT_NAME_MAIN}.app
                -qmldir=${CMAKE_CURRENT_SOURCE_DIR}
                -dmg)
