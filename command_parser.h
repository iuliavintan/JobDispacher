#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stddef.h>

typedef enum Command{
    PRIMES=1,
    PRIMEDIVISORS,
    ANAGRAMS
} Command;

typedef struct Job{
    char id[32];
    Command command;
    long long n;
    char name[9];
}Job;

typedef enum LineType{
    EMPTY=0,
    JOB,
    INVALID,
    WAIT
} LineType;

typedef struct ParsedLine{
    LineType type;
    int seconds;
    Job job;
}ParsedLine;

LineType parse_line(const char *line, ParsedLine *parsed_line);

#endif