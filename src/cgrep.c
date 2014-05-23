#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/types.h>
#include<errno.h>
#include<regex.h>

int exit_status = 0;
typedef struct options{
   uint16_t insensitive : 1;
   uint16_t filename_only : 1;
   uint16_t number_lines : 1;
   uint16_t reverse_match : 1;
   uint16_t use_stdin : 1;
   uint16_t numfiles : 11;
   char *pattern;
   char **files;
}options;

options *scan_opts(int argc, char **argv){
   int ix;
   options *myopts = calloc(1,sizeof(options));

   for(;;){
      int option = getopt(argc, argv, "ilnv");
      if(option == EOF) break;
      switch(option){
         char opstr[16];
         case 'i': myopts->insensitive = 1;
                   break;
         case 'l': myopts->filename_only = 1;
                   break;
         case 'n': myopts->number_lines = 1;
                   break;
         case 'v': myopts->reverse_match = 1;
                   break;
         default:
                   sprintf(opstr, "-%c", optopt);
                   fprintf(stderr, "%s invalid option\n", opstr);
                   exit(2);
      }
   }
   if (optind >= argc) {
      fprintf(stderr, "Expecting a pattern for search\n");
      exit(2);
   } 
   myopts->pattern = argv[optind++];
   if (optind == argc) myopts->use_stdin = 1;
   else {
      myopts->numfiles = argc - optind;
      myopts->files = calloc(myopts->numfiles, sizeof(char *));
      for (ix = optind; ix < argc ; ++ix)
         myopts->files[ix-optind] = argv[ix]; 
   }
   return myopts;
}

void del_opts(options **opt_p){
   options* opt = *opt_p;
   if(opt->numfiles) free(opt->files);
   free(opt);
   opt = NULL;
}


void grep(regex_t *regex, options *opts, FILE *fp, char *filename){
   char *line = NULL;
   size_t len = 0;
   int reti;
   int linenr = 1;

   //need to fix logic here
   while(getline(&line, &len, fp) != EOF){
      reti = regexec(regex, line, 0, NULL, 0);
      if (reti == 0 && !opts->reverse_match)
         printf("%d:%s", linenr, line);
      else if (reti == REG_NOMATCH && opts->reverse_match) 
         puts("No match");
      else{
         regerror(reti, regex, line, len);
         fprintf(stderr, "Regex match failed: %s\n", line);
         exit(2);
      }
      linenr;;
   }
}

//extern int errno;
int main(int argc, char **argv){
   FILE * fp;
   options *opts = scan_opts(argc, argv);
   regex_t regex;
   int reti;
   int ix;
   printf("case %d %d %d\n" ,
         opts->insensitive ,
         opts->insensitive ? REG_ICASE : 0,
         REG_ICASE);

   reti = regcomp(&regex, opts->pattern,
         opts->insensitive ? REG_ICASE : 0);
   if (reti) {
      fprintf(stderr, "Could not compile regex\n");
      exit(2);
   }

   if(opts->use_stdin) {
      grep(&regex, opts, stdin, "(standard input)");
   } else{
      for(ix = 0; ix < opts->numfiles; ++ix){
         if((fp = fopen(opts->files[ix], "r")) != NULL){
            grep(&regex, opts, fp, opts->files[ix]);
         } else{
           // fprintf(stderr, "%s: %s: No such file or directory\n",
            //      argv[0], opts->files[ix]);
            fprintf(stderr, "%s: %s: %s\n",
                  argv[0], opts->files[ix], strerror(errno));
         }
      }
   }

   del_opts(&opts);
   return EXIT_SUCCESS;
}
