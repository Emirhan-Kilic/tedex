#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

//data and funcions
struct termios ogTermios;
void exitRawMode();
void rawMode();
void errorAndExit();



//Terminal
void errorAndExit() {
    const char* errorMsg = "\nError: tcgetattr failed\n";
    write(STDERR_FILENO, errorMsg, sizeof(errorMsg) - 1);
    exit(EXIT_FAILURE);
}

void rawMode() {

    if ( (tcgetattr(STDIN_FILENO, &ogTermios)) != 0){
        errorAndExit();
    }


    if (atexit(exitRawMode) != 0) {
        errorAndExit();
    }

    struct termios raw = ogTermios;

    // traditional raw options
    raw.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF |ICRNL);
    raw.c_oflag &= ~OPOST;
    raw.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    raw.c_cflag &= ~(CSIZE | PARENB);
    raw.c_cflag |= CS8;

    //read with timeout tenths of a second, noncanonical
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        errorAndExit();
    }
}


void exitRawMode() {
    if ( (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ogTermios ) != 0)){
        errorAndExit();
    }
}




//init

int main(void) {
    rawMode();

    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
            errorAndExit();
        }
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q') break;
    }
    return 0;
}