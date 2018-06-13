// for starting 2 disksims in master-slave mode

#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(int argc, char **argv) {
  int pipes[2];
  int status;


  pipe(&pipes);

  if(argc != 15) {
    fprintf(stderr, "Usage: start <disksim1 wd> <disksim1 args> <disksim2 wd> <disksim2 args>\n");
    exit(1);
  }

  if(!fork()) {
    int c;
    char *argv2[8];
    for(c = 0; c < 6; c++){
      argv2[c] = argv[c+2];
    }
    argv2[6] = "master";
    argv2[7] = 0;
    
    dup2(pipes[0], 8);
    dup2(pipes[1], 9);
    close(pipes[0]);
    close(pipes[1]);

    if(chdir(argv[1])) {
      fprintf(stderr, "*** disksim1 couldn't chdir %s!\n", argv[1]);
      exit(1);
    }

    execv(*argv2, argv2);
    fprintf(stderr, "*** disksim1 failed %s!\n", *argv2);
    exit(1);
  }

  if(!fork()) {
    int c;
    char *argv2[8];
    for(c = 0; c < 6; c++){
      argv2[c] = argv[c+9];
    }
    argv2[6] = "slave";
    argv2[7] = 0;

    dup2(pipes[0], 8);
    dup2(pipes[1], 9);
    close(pipes[0]);
    close(pipes[1]);

    if(chdir(argv[8])) {
      fprintf(stderr, "*** disksim2 couldn't chdir %s!\n", argv[8]);
      exit(1);
    }

    execv(*argv2, argv2);
    fprintf(stderr, "*** disksim2 failed %s!\n", *argv2);
    exit(1);
  }

  close(pipes[0]);
  close(pipes[1]);


  wait(&status);
  wait(&status);
}
