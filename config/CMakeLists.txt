cmake_minimum_required(VERSION 3.2)
project(QuizBit C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wpedantic -pedantic-errors -Wformat -Wformat-security -Wuninitialized -Wshadow -Wsign-compare -Wmissing-field-initializers")
# -Wconversion
find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

set(SOURCES
        ../src/main.c
        ../src/db/database.c
        ../src/db/questions.c
        ../src/core/account.c
        ../src/core/gamecore.c
        ../src/core/userprofile.c
        ../src/utils/authutil.c
        ../src/utils/gameutil.c
        ../src/utils/utilities.c
        ../src/console/terminal.c
)

set(HEADERS
        ../include/console.h
        ../include/db.h
        ../include/emojis.h
        ../include/global.h
        ../include/playerstats.h
        ../include/utilities.h
)

add_executable(main ${HEADERS} ${SOURCES})

target_link_libraries(main PRIVATE ${SQLite3_LIBRARIES} ${OPENSSL_LIBRARIES} m)

target_include_directories(main PRIVATE include ${SQLite3_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})