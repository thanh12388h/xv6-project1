#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char buf[512];
  char *params[MAXARG];
  char c;
  int n, i = 0; // Khởi tạo i = 0

  if(argc < 2){
    printf("Usage: xargs command [args...]\n");
    exit(1);
  }

  // Copy các đối số từ lệnh gốc (ví dụ: echo bye)
  for(int j = 1; j < argc; j++) {
    params[j-1] = argv[j];
  }

  while((n = read(0, &c, 1)) > 0) { // Đọc từng ký tự 
    if(c == '\n') {
      buf[i] = 0; // Kết thúc chuỗi dòng hiện tại
      if(fork() == 0) { // Tiến trình con [cite: 36]
        params[argc-1] = buf;
        params[argc] = 0;
        exec(params[0], params); // Thực thi lệnh [cite: 36]
        exit(0);
      }
      wait(0); // Cha đợi con [cite: 37]
      i = 0; // Reset để đọc dòng tiếp theo
    } else {
      if (i < sizeof(buf) - 1) {
        buf[i++] = c;
      }
    }
  }
if(i > 0){ // Nếu vẫn còn dữ liệu trong buf mà chưa gặp \n
    buf[i] = 0;
    if(fork() == 0){
      params[argc-1] = buf;
      params[argc] = 0;
      exec(params[0], params);
      exit(0);
    }
    wait(0);
  }
  exit(0);
}
