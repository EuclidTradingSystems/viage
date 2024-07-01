# Copyed from https://github.com/christopro/AwesomeWorld-Cmake/blob/main/AwesomeWorld/deploy-qt-windows.cmake.in

set(WINDEPLOYQT "C:/Qt/6.6.2/mingw_64/bin/windeployqt.exe")
set(COMPONENT_NAME_MAIN "viage")
set(CMAKE_CURRENT_SOURCE_DIR "Z:/viage/client")

execute_process(COMMAND ${WINDEPLOYQT} --qmldir ${CMAKE_CURRENT_SOURCE_DIR}
                ${COMPONENT_NAME_MAIN}/data/bin
                WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/packages)
