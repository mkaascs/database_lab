cmake_minimum_required(VERSION 3.28)
project(1 C)

set(CMAKE_C_STANDARD 11)

add_executable(1 lab_db.c
        HeaderFiles/db_records.h
        HeaderFiles/db_commands.h
        SourceFiles/db_commands.c
        HeaderFiles/db_stats.h
        SourceFiles/db_stats.c
        HeaderFiles/db_command_parser.h
        SourceFiles/db_command_parser.c
)
