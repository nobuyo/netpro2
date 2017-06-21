#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <ncurses.h>
#include "mtalk2.h"
#include "session.h"

#define MYNAME_LENGTH 12
#define SEND_WIN_WIDTH 60
#define SEND_WIN_HEIGHT 1
#define RECV_WIN_WIDTH 60
#define RECV_WIN_HEIGHT 13

static char send_buf[BUF_LEN];
static char *input_buf;
static char recv_buf[BUF_LEN];
static char replaced_buf[BUF_LEN * 3];
static int soc;
static fd_set mask;
static int width;
static struct sockaddr_in server;
static struct sockaddr_in me;
static struct sockaddr_in from;
static int fromlen;
static char myname[MYNAME_LENGTH];
static int my_slot;

static WINDOW *win_send, *win_recv;
static WINDOW *frame_send, *frame_recv;

static void die();
static int login();
static void logout();

int session_setupclient(char *hostname, in_port_t port)
{
    struct hostent *server_ent;

    if ((server_ent = gethostbyname(hostname)) == NULL) {
        perror("gethostbyname");
        return -1;
    }

    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy((char *)&server.sin_addr, server_ent->h_addr, server_ent->h_length);

    if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    memset((char *)&me, 0, sizeof(me));
    me.sin_family = AF_INET;
    me.sin_addr.s_addr = htonl(INADDR_ANY);
    me.sin_port = 0;

    if (bind(soc, (const struct sockaddr *)&me, sizeof(me)) == -1) {
        perror("bind");
        return -1;
    }
    printf("successfully bound.\n");
    return login();
}

static int login()
{
    send_buf[0] = LOGIN;
    printf("Your name?\n");
    fgets(myname, MYNAME_LENGTH, stdin);
    chop_newline(myname, MYNAME_LENGTH);

    strcpy(send_buf + 1, myname);
    sendto(soc, send_buf, strlen(send_buf) + 1, 0, (const struct sockaddr *)&server, sizeof(server));
    fromlen = sizeof(from);
    recvfrom(soc, recv_buf, BUF_LEN, 0, (struct sockaddr * __restrict__)&from, &fromlen);

    if ((u_char)recv_buf[0] == CANNOT_LOGIN) {
        printf("cannot login.\n");
        return -1;
    } else if ((u_char)recv_buf[0] == LOGIN_OK) {
        sscanf(recv_buf + 1, "%20d", &my_slot);
        send_buf[0] = DATA;
        sprintf(send_buf + 1, "%-10s-> ", myname);
        input_buf = send_buf + 14;
        printf("You have logged in.\n");
        return 1;
    } else {
        printf("recv_buf[0] == %u\n", (u_char)recv_buf[0]);
        return -2;
    }
}

void session_init()
{
    width = soc + 1;
    FD_ZERO(&mask);
    FD_SET(0, &mask);
    FD_SET(soc, &mask);

    initscr();
    signal(SIGINT, die);

    frame_send = newwin(SEND_WIN_HEIGHT + 2, SEND_WIN_WIDTH + 2, 18, 0);
    win_send = newwin(SEND_WIN_HEIGHT, SEND_WIN_WIDTH, 19, 1);
    box(frame_send, '|', '-');
    scrollok(win_send, TRUE);
    wmove(win_send, 0, 0);

    frame_recv = newwin(RECV_WIN_HEIGHT + 2, RECV_WIN_WIDTH + 2, 0, 0);
    win_recv = newwin(RECV_WIN_HEIGHT, RECV_WIN_WIDTH, 1, 1);
    box(frame_recv, '|', '-');
    scrollok(win_recv, TRUE);
    wmove(win_recv, 0, 0);

    cbreak();
    noecho();

    wrefresh(frame_recv);
    wrefresh(win_recv);
    wrefresh(frame_send);
    wrefresh(win_send);
}

void session_loop()
{
    int c;
    int flag = 1;
    fd_set readOk;
    int len = 0;
    int i,j;
    int y, x;
    int n;
    int count = 0;

    while (1) {
        readOk = mask;
        select(width, (fd_set *)&readOk, NULL, NULL, NULL);

        // has input from keyboard
        if (FD_ISSET(0, &readOk)) {
            c = getchar();

            // backspace
            if (c == '\b' || c == 0x10 || c == 0x7F) {
                if (len == 0) {
                    continue;
                }
                len--;
                getyx(win_send, y, x);
                wmove(win_send, y, x-1);
                waddch(win_send, ' ');
                wmove(win_send, y, x-1);
            }
            else if (c == '\n' || c == '\r') {
                input_buf[len++] = '\n';
                sendto(soc, send_buf, len + 14, 0, (const struct sockaddr *)&server, sizeof(server));

                // Clearing the send window
                wclear(win_send);
                len = 0;
            }
            else {
                input_buf[len++] = c;
                waddch(win_send, c);
            }
            wrefresh(win_send);
        }

        // if data in socket
        if (FD_ISSET(soc, &readOk)) {
            fromlen = sizeof(from);
            n = recvfrom(soc, recv_buf, BUF_LEN, 0, (struct sockaddr * __restrict__)&from, &fromlen);
            if ((u_char)recv_buf[0] == DATA) {
                for (i = 1,j = 0; i < n; i++,j++) {
                    if (i < n-1 && recv_buf[i] == ':') {
                        switch(recv_buf[i+1]) {
                            case 'D':
                                j = replace(replaced_buf, "(^_^)", j);
                                i+=2;
                                count+=3;
                                break;
                            case 'X':
                                j = replace(replaced_buf, "(T.T)", j);
                                i+=2;
                                count+=3;
                                break;
                            case 'O':
                                j = replace(replaced_buf, "(^O^)", j);
                                i+=2;
                                count+=3;
                                break;
                            case '(':
                                j = replace(replaced_buf, "(*_*)", j);
                                i+=2;
                                count+=3;
                                break;
                            default:
                                break;
                        }
                    }
                    
                    replaced_buf[j] = recv_buf[i];
                }

                // for (i = 0; i < n; i++) {
                //     waddch(win_recv, recv_buf[i]);
                // }
                for (i = 0; i < n + count - 1; i++) {
                    waddch(win_recv, replaced_buf[i]);
                }
                clear_buf(replaced_buf, BUF_LEN * 3);
                count = 0;

            }
            else if ((u_char)recv_buf[0] == END) {
                flag = 0;
            }

            wrefresh(win_recv);

            wrefresh(win_send);
        }

        if (flag == 0) break;
    }

    die();
}

int replace(char *replaced, char *face, int n)
{
    int i = 0;
    int c = n;
    while(face[i] != '\0') {
        replaced[c] = face[i];
        i++;
        c++;
    }

    return c;
}

void clear_buf(char *buf, int n) {
    int i;
    for (i = 0; i < n; i++) {
        buf[i] = '\0';
    }
}

static void die()
{
    endwin();
    logout();
    close(soc);
    exit(0);
}

static void logout()
{
    send_buf[0] = LOGOUT;
    sprintf(send_buf + 1, "%20d\n", my_slot);
    sendto(soc, send_buf, 4, 0, (const struct sockaddr *)&server, sizeof(server));
}
