

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  struct procinfo info;

  // Test process hiện tại
  if(procinfo(getpid(), &info) == 0) {
    printf("=== Current Process ===\n");
    printf("Process: %s\n", info.name);
    printf("PID: %d, PPID: %d\n", info.pid, info.ppid);
    printf("State: %d\n", info.state);
    printf("Memory: %ld bytes\n", info.sz);
  }

  // Test fork - query child process
  printf("\n=== Fork Test ===\n");
  int pid = fork();
  if(pid == 0) {
    sleep(10);  // child ngủ để parent kịp query
    exit(0);
  } else {
    if(procinfo(pid, &info) == 0) {
      printf("Child  - Name: %s, PID: %d, PPID: %d, State: %d\n",
             info.name, info.pid, info.ppid, info.state);
    }
    if(procinfo(getpid(), &info) == 0) {
      printf("Parent - Name: %s, PID: %d, PPID: %d, State: %d\n",
             info.name, info.pid, info.ppid, info.state);
    }
    wait(0);
  }

  // Test query PID tùy chỉnh từ argument
  if(argc > 1) {
    int qpid = atoi(argv[1]);
    printf("\n=== Query PID %d ===\n", qpid);
    if(procinfo(qpid, &info) == 0) {
      printf("Process: %s\n", info.name);
      printf("PID: %d, PPID: %d\n", info.pid, info.ppid);
      printf("State: %d\n", info.state);
      printf("Memory: %ld bytes\n", info.sz);
    } else {
      printf("procinfo(%d) failed — PID not found\n", qpid);
    }
  }

  // Test PID không tồn tại
  printf("\n=== Invalid PID Test ===\n");
  if(procinfo(9999, &info) < 0)
    printf("procinfo(9999) correctly returned -1\n");

  return 0;
}