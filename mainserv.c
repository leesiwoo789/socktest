#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

#define MAXLINE 511
#define MAX_SOCK 1024
#define PORT 3305
#define IPADDR "172.24.77.66"

char *EXIT_STRING = "exit";
char *START_STRING = "Connected to chat_server \n";
int maxfdp1;
int num_chat = 0;
int same = 1;
int clisock_list[MAX_SOCK];
int listen_sock;
int welcomecheck = 0;
int play = 0;
int round1 = 0;
int round1max = 0;
int round1chat = 0;
int max = 0;
int round1win = 0;
int tmp = 0;
int round1notice = 0;
int score[100];
int scoremax = 0;
int money[100];
int number = 1;
int end = 1;
void addClient(int s, struct sockaddr_in *newcliaddr);
void removeClient(int);
int set_nonblock(int sockfd);
int is_nonblock(int sockfd);
int tcp_listen(int host, int port, int backlog);

void errquit(char *mesg){
    perror(mesg);
    exit(1);
}

int main(int argc, char *argv[]){
    struct sockaddr_in cliaddr;
    char buf[MAXLINE];
    int i, j, k, nbyte, accp_sock, clilen, count;
    int check = 1;
    int rulecheck = 0;
    int c[num_chat];
    char member[20];
    char save[100];
    char result[100];
    char rmoney[100];
    char rule[] = "게임시작에 필요한 인원 2명 이상, 현재 : ";
    for(k = 0; k < 100; k++){
        score[k] = 0;
        money[k] = 100;
    }

    for(k = 0; k < num_chat; k++){
        c[k] = 0;
    }

    if(argc != 2){
        printf("사용법 : %s port\n", argv[0]);
        exit(0);
    }

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);

    if(listen_sock < 0)
        errquit("tcp_listen fail");
    if(set_nonblock(listen_sock) < 0)
        errquit("set_nonblock fail");
    for(count = 0; ; count++){
        if(count == 100000){
            fflush(stdout);
            count = 0;
        }
        clilen = sizeof(cliaddr);
        accp_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &clilen);
        if(accp_sock == -1 && errno != EWOULDBLOCK)
            errquit("accept fail");
        else if(accp_sock > 0){
            clisock_list[num_chat] = accp_sock;
            if(is_nonblock(accp_sock) != 0 && set_nonblock(accp_sock) < 0)
                errquit("set_nonblock fail");
            addClient(accp_sock, &cliaddr);
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("%d번째 사용자 추가.\n", num_chat);
        }
        for(i = 0; i < num_chat; i++){
            errno = 0;
            if(welcomecheck == 0){ //처음으로 들어온사람에게 뿌려지는 메세지
                welcomecheck = 1;
                char rules[100];
                sprintf(rules, "g를 누르면 게임시작, 2명 이상필요 현재 %d명", num_chat);
                send(clisock_list[i], rules, 100, 0);
            }
            nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
            if(nbyte == 0){
                removeClient(i);
                continue;
            }
            else if(nbyte == -1 && errno == EWOULDBLOCK)
                continue;
            if(strstr(buf, EXIT_STRING) != NULL){
                removeClient(i);
                continue;
            }
            check = strncmp(buf, "g", 1);
            buf[nbyte] = 0;
            if(num_chat >= 2 && check == 0){ //게임시작
                for(j = 0; j < num_chat; j++){
                    send(clisock_list[j], "\n게임 시작", 20, 0);
                    char playernum[100];
                    sprintf(playernum, "\n%d번째 사람", j);
                    send(clisock_list[j], playernum, 20, 0);
                }
                play = 1;
            }
            else if(check == 0){ //게임시작 인원부족할 때
                sprintf(member, "%d", num_chat);
                if(rulecheck == 0){
                    strcat(rule, member);
                    strcat(rule, "명");
                }
                send(clisock_list[i], rule, 100, 0);
                rulecheck = 1;
            }
            else{ //채팅
                for(j = 0; j < num_chat; j++)
                    if((clisock_list[i] != clisock_list[j])){
                        if(buf[0] >=  48 && buf[0] <= 57){
                        }
                        else{
                            send(clisock_list[j], buf, nbyte, 0);
                            printf("%s\n", buf);
                        }
                    }
            }
 //게임시작-----------------------------------------------------
            for(j = 0; j < num_chat; j++){
                if(play == 1 && round1 == 0 && round1notice < num_chat){
                    char buy[100];
                    sprintf(buy, "\n%d번째 경매, 입찰가 입력 예) : 60, 기본소지금 : 100원", number);
                    send(clisock_list[j], buy, 200, 0);
                    round1notice = round1notice + 1;
                }
            }
            if(play == 1 && round1 == 0){
                if(buf[0] >= 48 && buf[0] <= 57){
                    round1chat = round1chat + 1;
                    c[i] = atoi(buf);
                    printf("\n입찰가(%d번 사람) : %d", i, c[i]);
                }
                if(round1chat == num_chat){
                    round1 = 1;
                }
            }
            if(round1 == 1){
                for(j = 0; j < num_chat; j++){
                    if(money[j] - c[j] < 0){
                        same = 0;
                        printf("\n소지금보다 많은 가격 입찰됨. 유찰!!\n");
                        for(k = 1; k < num_chat; k++){
                            send(clisock_list[0], "\n소지금보다 많은가격 입찰됨, 다시g키를 눌러 시작.", 100, 0);
                            send(clisock_list[k], "\n소지금보다 많은가격 입찰됨, 곧 다시시작됨", 100, 0);
                        }
                        break;
                    }
                }
            }
            for(j = 0; j < num_chat; j++){ //경매후 높은가격입찰한사람
                if(same == 0)
                    break;
                if(round1 == 1){
                    if(c[j] > round1max){
                        round1max = c[j];
                        tmp = j;
                    }
                    else if(c[j] == round1max){
                        same = 0;
                        printf("\n같은가격 입찰됨. 유찰!!\n");
                        for(j = 1; j < num_chat; j++){
                            send(clisock_list[0], "\n같은 가격 입찰됨. 유찰!! 다시 g키를 눌러 시작.", 100, 0);
                            send(clisock_list[j], "\n같은 가격 입찰됨 곧 다시 시작됨 ", 100, 0);
                        }
                        break;
                    }
                }
            }
            if(round1 == 1 && same == 1){
                sprintf(save, "\n낙찰자 : %d, 낙찰금액 : %d", tmp, round1max);
                printf("\n낙찰자 : %d, 낙찰금액 : %d\n", tmp, round1max);
                money[tmp] = money[tmp] - round1max;
                score[tmp] = score[tmp] + 1;
                for(j = 0; j < num_chat; j++){
                    if(same == 0)
                        break;
                    send(clisock_list[j], save, 100, 0);
                    for(k = 0; k < num_chat; k++){
                        if(score[k] >= scoremax){
                            scoremax = score[k];
                        }
                    }
                    for(k = 0; k < num_chat; k++){
                        sprintf(result, "%d번 사람, 점수 : %d점", k, score[k]);
                        send(clisock_list[j], result, 100, 0);
                    }
                    printf("\n%d번 사람, 점수 : %d점", j, score[j]);
                    printf("\n%d번 사람, 남은 소지금 : %d점\n", j, money[j]);
                    sprintf(rmoney, "\n남은소지금 : %d", money[j]);
                    send(clisock_list[j], rmoney, 100, 0);
                }
                number = number + 1;
                round1 = 0;
                round1max = 0;
                welcomecheck = 0;
                play = 0;
                round1chat = 0;
                max = 0;
                round1win = 0;
                tmp = 0;
                round1notice = 0;
                scoremax = 0;
                for(j = 0; j < num_chat; j++){
                    if(money[j] <= 0){
                        for(k = 0; k < num_chat; k++){
                            send(clisock_list[k], "\n끝", 100, 0);
                        }
                    }
                    int winmax = 0;
                    int winnum = 0;
                    char win[100];
                    if(score[j] >= winmax){
                        max = score[j];
                        winnum = j;
                    }
                    if(money[j] <= 0){
                        end = 0;
                        sprintf(win, "\n%d번 %d점으로 승리!!", winnum, max);
                        printf("\n%d번 %d점으로 승리!!\n", winnum, max);
                        for(k = 0; k < num_chat; k++){
                            send(clisock_list[k], win, 100, 0);
                            send(clisock_list[k], "q", 1, 0);
                            if(end == 0) break;
                        }
                    }
                    if(end == 0) break;
                }
            }
            else if(same == 0){
                number = number + 1;
                round1 = 0;
                round1max = 0;
                welcomecheck = 0;
                play = 0;
                round1chat = 0;
                max = 0;
                round1win = 0;
                tmp = 0;
                round1notice = 0;
                scoremax = 0;
                same = 1;
                for(k = 0; k < 100; k++){
                    c[k] = 0;
                }
                break;
            }
            if(end == 0) break;
        }
         if(end == 0) break;
    }
}

void addClient(int s, struct sockaddr_in *newcliaddr){
    char buf[20];
    inet_ntop(AF_INET, &newcliaddr -> sin_addr, buf, sizeof(buf));
    printf("new client : %s\n", buf);

    clisock_list[num_chat] = s;
    num_chat++;
    welcomecheck = 0;
}

void removeClient(int i){
    close(clisock_list[i]);
    if(i != num_chat - 1)
        clisock_list[i] = clisock_list[num_chat - 1];
    num_chat--;
    printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d\n", num_chat);
}

int is_nonblock(int sockfd){
    int val;
    val = fcntl(sockfd, F_GETFL, 0);
    if(val & O_NONBLOCK)
        return 0;
    return -1;
}

int set_nonblock(int sockfd){
    int val;
    val = fcntl(sockfd, F_GETFL, 0);
    if(fcntl(sockfd, F_SETFL, val | O_NONBLOCK) == -1)
        return -1;
    return 0;
}
int tcp_listen(int host, int port, int backlog){
    int sd;
    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1){
        perror("socket fail");
        exit(1);
    }
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(host);
    servaddr.sin_port = htons(port);
    if(bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("bind fail"); exit(1);
    }
    listen(sd, backlog);
    return sd;
}
