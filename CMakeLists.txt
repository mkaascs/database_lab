cmake_minimum_required(VERSION 3.28)
project(1 C)

set(CMAKE_C_STANDARD 11)

add_executable(1 lab_db.c
        application/domain/models.h
        application/commands.h
        memory/stats.h
        memory/stats.c
        application/parsing/parser.h
        application/parsing/parser.c
        application/commands.c
        application/commands.c
        application/domain/converting/converter.h
        application/domain/converting/converter.c
        application/domain/models.c
        application/operations/fields.h
        application/operations/fields.c
        application/operations/conditions/conditions.h
        application/operations/conditions/conditions.c
        application/operations/conditions/types/types.h
        application/operations/conditions/types/types.c
        application/operations/sort/sort.h
        application/operations/sort/sort.c
)
