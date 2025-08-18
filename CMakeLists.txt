cmake_minimum_required(VERSION 3.16)

project(Tic-tac-toe VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Настройка флагов компилятора
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

# Сначала попробуем найти системную SFML
find_package(SFML 3.0 COMPONENTS system window graphics audio QUIET)

if(NOT TARGET sfml-graphics)
    message(STATUS "Системная SFML не найдена, собираем локально...")
    
    # Проверяем наличие локальной SFML
    set(SFML_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SFML)
    if(NOT EXISTS ${SFML_SOURCE_DIR}/CMakeLists.txt)
        message(FATAL_ERROR 
            "SFML не найдена в ${SFML_SOURCE_DIR}!\n"
            "Клонируйте SFML:\n"
            "  git submodule add https://github.com/SFML/SFML.git third_party/SFML\n"
            "  git submodule update --init --recursive\n"
            "Или скачайте исходники с GitHub и распакуйте в third_party/SFML/")
    endif()
    
    # Настройки для сборки SFML
    message(STATUS "Настройка локальной сборки SFML...")
    
    # Отключаем ненужные компоненты SFML для ускорения сборки
    set(SFML_BUILD_AUDIO TRUE CACHE BOOL "Build audio module")
    set(SFML_BUILD_GRAPHICS TRUE CACHE BOOL "Build graphics module") 
    set(SFML_BUILD_WINDOW TRUE CACHE BOOL "Build window module")
    set(SFML_BUILD_SYSTEM TRUE CACHE BOOL "Build system module")
    set(SFML_BUILD_NETWORK FALSE CACHE BOOL "Build network module")
    
    # Отключаем примеры и тесты
    set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "Build SFML examples")
    set(SFML_BUILD_DOC FALSE CACHE BOOL "Build SFML documentation")
    set(SFML_BUILD_TEST_SUITE FALSE CACHE BOOL "Build SFML test suite")
    
    # Настройки линковки
    option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
    
    # Подавляем предупреждения от SFML
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
    endif()
    
    # Добавляем SFML как субпроект
    add_subdirectory(${SFML_SOURCE_DIR})
    
    # Восстанавливаем флаги предупреждений для нашего проекта
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /W4")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
    endif()
    
    message(STATUS "SFML будет собрана локально из: ${SFML_SOURCE_DIR}")
else()
    message(STATUS "Используется системная SFML")
endif()

# Создание исполняемого файла
add_executable(Tic-tac-toe ${SOURCES} ${HEADERS})

# Включение заголовков проекта
target_include_directories(Tic-tac-toe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Линковка с SFML
target_link_libraries(Tic-tac-toe PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
)

# Копирование шрифта
add_custom_command(TARGET Tic-tac-toe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FONT_SOURCE}" "${FONT_DESTINATION}"
    COMMENT "Copying font file..."
)

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
message(STATUS "Статическая сборка: ${BUILD_SHARED_LIBS}")
if(TARGET sfml-graphics)
    message(STATUS "SFML targets доступны")
endif()
message(STATUS "===============================")