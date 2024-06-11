#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

#define COL 50
#define ROW 20

int main(){
  int quit = 0;

  int x[1000], y[1000];
  

  // hide cursor 
  printf("\e[?25l");

  struct termios old_t, new_t;
  tcgetattr(STDIN_FILENO, &old_t);
  new_t = old_t;
  new_t.c_lflag &= ~(ICANON | ECHO );

  tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
  while ( !quit ) {

    for ( int i = 0; i < ROW; i++){
      for ( int j = 0; j < COL; j++){
        printf(" ");
      }
      printf("\n");
    }
  printf("\e[%iA", ROW );

    int head = 0; int tail = 0;
    int xdir = 1, ydir = 0;
    x[head] = COL / 2;
    y[head] = ROW / 2;
    int gameover = 0;
    int applex = -1, appley;

    while ( !quit && !gameover ) {
      if ( applex < 0) {
        applex = rand() % COL;
        appley = rand() % ROW;
      

      for ( int i = tail; i != head; i = ( i + 1 ) % 1000){
        if ( x[i] == applex && y[i] == appley ) {
            applex = -1;
          }

        }

        if ( applex >= 0 ) {
          printf("\e[%iB\e[%iC\033[31m❤", appley, applex);
          printf("\e[%iF", appley);

        }

      }
       
      //clear snake tail

      printf("\e[%iB\e[%iC ", y[tail], x[tail]);
      printf("\e[%iF", y[tail]);
      
      if ( x[head] == applex && y[head] == appley ) {
        applex = -1;
        printf("\a");
      }else{
        tail = ( tail + 1 ) % 1000;
      }
     

      int newhead = (head + 1) % 1000;
      x[newhead] = (x[head] + xdir + COL ) % COL;
      y[newhead] = (y[head] + ydir + ROW ) %  ROW;
      head = newhead;

      for ( int i = tail; i != head; i = ( i + 1 ) % 1000){
        if ( x[i] == x[head] && y[i] == y[head] ) {
          gameover = 1;
        }

      }


      //draw head
      printf("\e[%iB\e[%iC\033[32m", y[head], x[head]);
      printf("\e[%iF", y[head]);
      fflush(stdout);

      usleep(5000000 / 50 );
     
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 0;

      fd_set  fds;

      FD_ZERO(&fds);
      FD_SET(STDIN_FILENO, &fds);

      select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

      if ( FD_ISSET(STDIN_FILENO, &fds)){ 
      int c = getchar();
        if ( c == 27 || c == 'q' ){ 
          quit = 1;
        }
        else if ( c == 'a' && xdir != 1 ){
          xdir = -1;
          ydir = 0;
        }else if ( c == 'd' && xdir != -1 ){
          xdir = 1;
          ydir = 0;
        }else if ( c == 'w' && ydir != 1 ){
          xdir = 0;
          ydir = -1;
        }else if ( c == 's' && ydir != -1 ){
          xdir = 0;
          ydir = 1;
        }

      }
   
    }
    if ( !quit ) {
      printf("\e[%iB\e[%iC Game over!", ROW / 2, COL / 2 - 5);
      printf("\e[%iF", ROW / 2);
      fflush(stdout);
      getchar();

    }


  }


  
// show cursor again
  printf("\e[?25h");
  tcsetattr(STDIN_FILENO, TCSANOW, &old_t);
return 0;
}
