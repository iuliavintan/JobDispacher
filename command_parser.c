#include"command_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>

static void rstrip(char *s){
    size_t len=strlen(s);
    while(len>0 && (s[len-1]=='\n' || s[len-1]=='\r' || isspace((unsigned char)s[len-1]))){
        s[len-1]='\0';
        len--;
    }
}

static  char *skip_spaces( char *str){
    while(*str && isspace((unsigned char)*str)){
        str++;
    }
    return str;
}

int is_empty(char *str){
    char *s=skip_spaces(str);
    return *s=='\0';
}

static int parse_int(const char *str, int *out){
    char *end=NULL;
    long v=strtol(str, &end, 10);
    if(end==str) return 0;

    end=(char *)skip_spaces(end);
    if(*end!='\0') return 0;
    if(v<0 /*|| v>24*3600*/) return 0;
    *out=(int)v;
    return 1;
}

static int parse_long_long(const char *str, long long *out){
    char *end=NULL;
    long long v=strtoll(str, &end, 10);
    if(end==str) return 0;

    end=(char *)skip_spaces(end);
    if(*end!='\0') return 0;
    if(v<0) return 0;
    *out=v;
    return 1;
}

//expected formats:
//<job_id> PRIMES <N>
//<job_id> PRIMEDIVISORS <N>
//<job_id> ANAGRAMS <name>
//WAIT <seconds>


LineType parse_line(const char *line, ParsedLine *parsed_line){
    if(line == NULL || parsed_line == NULL){
        return INVALID;
    }

    char buff[256];
    snprintf(buff, sizeof(buff), "%s", line);
    rstrip(buff);

    if(is_empty(buff)){
        parsed_line->type = EMPTY;
        return EMPTY;
    }

    char *t1=strtok(buff, " \t");
    if(t1==NULL){
        parsed_line->type=INVALID;
        return INVALID;
    }

    if(strcmp(t1, "WAIT")==0){
        char *t2=strtok(NULL, " \t");
        if(t2==NULL){
            parsed_line->type=INVALID;
            return INVALID;
        }
        int seconds=0;
        if(!parse_int(t2, &seconds)){
            parsed_line->type=INVALID;
            return INVALID;
        }

        char *extra=strtok(NULL, " \t");
        if(extra!=NULL){
            parsed_line->type=INVALID;
            return INVALID;
        }

        parsed_line->type=WAIT;
        parsed_line->seconds=seconds;
        return WAIT;
    }

    char *client= t1;
    char *cmd=strtok(NULL, " \t");
    char *arg=strtok(NULL, " \t");
    char *extra=strtok(NULL, " \t");

    if(cmd==NULL || arg==NULL || extra!=NULL){
        parsed_line->type=INVALID;
        return INVALID;
    }

    memset(&parsed_line->job, 0, sizeof(Job));
    snprintf(parsed_line->job.id, sizeof(parsed_line->job.id), "%s", client);

    if(strcmp(cmd, "PRIMES")==0){
        int N=0;
        if(!parse_int(arg, &N)){
            parsed_line->type=INVALID;
            return INVALID;
        }
        parsed_line->job.command=PRIMES;
        parsed_line->job.n=N;

    } else if(strcmp(cmd, "PRIMEDIVISORS")==0){
        long long N=0;
        if(!parse_long_long(arg, &N)){
            parsed_line->type=INVALID;
            return INVALID;
        }
        parsed_line->job.command=PRIMEDIVISORS;
        parsed_line->job.n=N;

    } else if(strcmp(cmd, "ANAGRAMS")==0){
        size_t name_len=strlen(arg);
        if(name_len>8){
            parsed_line->type=INVALID;
            return INVALID;
        }
        snprintf(parsed_line->job.name, sizeof(parsed_line->job.name), "%s", arg);
        parsed_line->job.command=ANAGRAMS;

    } else {
        parsed_line->type=INVALID;
        return INVALID;
    }
    parsed_line->type=JOB;
    return JOB;
}