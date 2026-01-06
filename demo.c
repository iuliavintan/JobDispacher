#include"functionalities.h"
#include"command_parser.h"
#include <stdio.h>
#include <stdlib.h>

char *pick_cmd(Command cmd){
    switch(cmd){
        case PRIMES:
            return "PRIMES";
        case PRIMEDIVISORS:
            return "PRIMEDIVISORS";
        case ANAGRAMS:
            return "ANAGRAMS";
        default:
            return "UNKNOWN";
    }
}

int main(int argc, char *argv[])
{
    int N=30;

    printf("Number of primes up to %d: %d\n", N, count_primes(N));

    long long M=452876;

    printf("Number of prime divisors of %lld: %d\n", M, prime_divisors(M));

    const char *name="abba";
    char *out=NULL;
    size_t length=0;
    int count=0;

    if(anagrams(name, &length, &out, &count)){
        printf("Anagrams of %s (total %d):\n%s", name, count, out);
        free(out);
    } else {
        printf("Failed to generate anagrams.\n");
    }

    if(argc !=2 ){
        perror("give argument file");
        exit(-1);
    }

    FILE *fp=fopen(argv[1], "r");
    if(fp==NULL){
        perror("fopen failed");
        exit(-1);
    }

    char line[256];

    while(fgets(line, sizeof(line), fp)!=NULL){
        ParsedLine pl;
        LineType lt=parse_line(line, &pl);
        switch(lt){
            case EMPTY:
                printf("Empty line\n");
                break;
            case INVALID:
                printf("Invalid line: %s", line);
                break;
            case WAIT:
                printf("WAIT command for %d seconds\n", pl.seconds);
                break;
            case JOB:
                printf("Job ID: %s, Command: %s, Argument: ",
                    pl.job.id,
                    pick_cmd(pl.job.command)
                );
                if(pl.job.command == PRIMES){
                    printf("%d\n", (int)pl.job.n);
                } else if(pl.job.command == PRIMEDIVISORS){
                    printf("%lld\n", pl.job.n);
                } else if(pl.job.command == ANAGRAMS){
                    printf("%s\n", pl.job.name);
                }
                break;
            default:
                printf("Unknown line type\n");
        }
    }

    fclose(fp);

    return 0;
}