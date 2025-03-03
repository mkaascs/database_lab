cmake_minimum_required(VERSION 3.28)
project(1 C)

set(CMAKE_C_STANDARD 11)

add_executable(1 lab_db.c
        application/domain/db_records.h
        application/db_commands.h
        application/db_commands.c
        memory/stats.h
        memory/stats.c
        parsing/db_command_parser.h
        parsing/db_command_parser.c
)
