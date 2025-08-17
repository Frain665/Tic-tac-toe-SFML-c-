cmake_minimum_required(VERSION 3.16)

project(Tic-tac-toe VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Настройка флагов компилятора в зависимости от типа компилятора
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# Переменные для шрифтов
set(FONT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/Fonts/DefaultFont/DefaultFont.ttf")
set(FONT_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources/Fonts/DefaultFont/DefaultFont.ttf")

# Включение директорий
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Исходные файлы
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/main_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/app.cpp
)

# Заголовочные файлы
file(GLOB_RECURSE HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

# Создание директории для ресурсов
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources/Fonts/DefaultFont")

# Поиск SFML
find_package(SFML 2.5 COMPONENTS system window graphics audio)

if(NOT SFML_FOUND)
    message(STATUS "SFML не найдена в системе, ищем в third_party...")
    set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SFML)
    
    if(EXISTS ${SFML_DIR})
        add_subdirectory(${SFML_DIR})
        set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics sfml-audio)
        set(SFML_INCLUDE_DIRS ${SFML_DIR}/include)
        message(STATUS "SFML найдена в third_party: ${SFML_DIR}")
    else()
        message(FATAL_ERROR "SFML не найдена! Установите SFML или поместите её в папку third_party/")
    endif()
else()
    message(STATUS "SFML найдена в системе")
    set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics sfml-audio)
endif()

# Создание исполняемого файла
add_executable(Tic-tac-toe ${SOURCES} ${HEADERS})

# Линковка библиотек
target_link_libraries(Tic-tac-toe ${SFML_LIBRARIES})

# Включение директорий SFML
if(SFML_INCLUDE_DIRS)
    target_include_directories(Tic-tac-toe PRIVATE ${SFML_INCLUDE_DIRS})
endif()

# Копирование шрифта (всегда, не только на Windows)
add_custom_command(TARGET Tic-tac-toe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FONT_SOURCE}" "${FONT_DESTINATION}"
    COMMENT "Copying font file..."
)

# Копирование DLL для Windows
if(WIN32 AND SFML_FOUND AND BUILD_SHARED_LIBS)
    foreach(lib ${SFML_LIBRARIES})
        if(TARGET ${lib})
            get_target_property(SFML_LIB_PATH ${lib} LOCATION)
            if(SFML_LIB_PATH)
                get_filename_component(SFML_LIB_DIR ${SFML_LIB_PATH} DIRECTORY)
                file(GLOB SFML_DLLS "${SFML_LIB_DIR}/*.dll")
                
                foreach(dll ${SFML_DLLS})
                    add_custom_command(TARGET Tic-tac-toe POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${dll} $<TARGET_FILE_DIR:Tic-tac-toe>
                        COMMENT "Copying SFML DLL: ${dll}"
                    )
                endforeach()
            endif()
        endif()
    endforeach()
endif()

# Установка
install(TARGETS Tic-tac-toe
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Определения для отладки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Tic-tac-toe PRIVATE DEBUG_BUILD=1)
    message(STATUS "Сборка в режиме отладки")
endif()

# Информационные сообщения
message(STATUS "=== Конфигурация проекта ===")
message(STATUS "Проект: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Компилятор: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Стандарт C++: ${CMAKE_CXX_STANDARD}")
message(STATUS "Тип сборки: ${CMAKE_BUILD_TYPE}")
message(STATUS "Исходные файлы: ${SOURCES}")
message(STATUS "SFML библиотеки: ${SFML_LIBRARIES}")
message(STATUS "===============================")