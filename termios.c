#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(){
  struct termios old_t, new_t;

  tcgetattr(STDIN_FILENO, &old_t);

  new_t = old_t;
  
  new_t.c_lflag &= ~(ICANON | ECHO );

  tcsetattr(STDIN_FILENO, TCSANOW, &new_t);

  char c;

  while ( read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
    printf("you typed: %c\n", c);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &old_t);

  return 0;

}
