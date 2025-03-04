cmake_minimum_required(VERSION 3.28)
project(1 C)

set(CMAKE_C_STANDARD 11)

add_executable(1 lab_db.c
        application/domain/db_records.h
        application/commands.h
        memory/stats.h
        memory/stats.c
        application/parsing/parser.h
        application/parsing/parser.c
        application/commands.c
        application/commands.c
        application/domain/converting/converter.h
        application/domain/converting/converter.c
)
