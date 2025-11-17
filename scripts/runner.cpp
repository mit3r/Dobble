#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <signal.h>
#include <string>
#include <sys/types.h>

pid_t *pids = nullptr;
int n = 0;

char *reset = (char *)"\033[0m";  // Reset
char* colors[] = {
  (char *)"\033[32m", // Green
  (char *)"\033[33m", // Yellow
  (char *)"\033[34m", // Blue
  (char *)"\033[35m", // Magenta
  (char *)"\033[36m"  // Cyan
};

void cleanup() {
  if (pids == nullptr || n <= 0) return;
  for (int i = 0; i < n; i++) {
    if (pids[i] > 0) {
      kill(pids[i], SIGTERM);
      waitpid(pids[i], NULL, 0);
    }
  }

  delete[] pids;
  pids = nullptr;
}

static void handle_sigint(int) {
  cleanup();
  _exit(0);
}

int main( int argc, char** argv) {

  if(argc == 1) {
    printf("Usage: %s <name1> <cmd1> ... <nameN> <cmdN>\n", argv[0]);
    return 0;
  }

  if ((argc-1) % 2 == 1) {
    printf("Wrong number of arguments. Provide an even number of arguments. \n");
    return 1;
  }

  n = (argc-1)/2;
  pids = new int[n];

  const char* name;
  const char* cmd;
  for (int i = 0; i < n; i++) {

    name = argv[2*i + 1];
    cmd = argv[2*i + 2];

    pid_t pid = fork();

    if (pid == 0) { // Child process
      
      char buffer[1024];
      char *c = colors[i % (sizeof(colors)/sizeof(colors[0]))];

      sprintf(buffer, "(%s) 2>&1 | sed -u 's/^/%s[%s]%s: /'", cmd, c, name, reset);
      execlp("sh", "sh", "-c", buffer, (char *)NULL);
      perror("execlp");
      _exit(1);
      return 0;
    } else if (pid > 0) { // Parent process
      pids[i] = pid;
    }
  }
  signal(SIGINT, handle_sigint);

  printf("Started %d processes. Press Ctrl+C to terminate them.\n", n);
  printf("---------------------------------------------------\n");
  pause();
  cleanup();
}

