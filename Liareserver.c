#include "GameMsg.h"

#define MAXLINE 511
#define MAX_SOCK 1024

char buf[MAXLINE + 1];
int num_chat = 0;
int max;
int userid[5];
int nvote[5] = { 0 };
int listen_sock;
int clisock_list[MAX_SOCK];
int i, nbyte;

int getmax();
void addClient(int s, struct sockaddr_in *newcliaddr);
void removeClient(int s);
int tcp_listen(int host, int port, int backlog);
int set_nonblock(int sockfd);
int is_nonblock(int sockfd);
void recvdmsg();
int maxfd =0;

void errquit(char *mesg)
{
	perror(mesg);
	exit(1);
}

int main(int argc, char *argv[]) {
	struct sockaddr_in cliaddr;
	int count;
	int j, accp_sock, clilen = sizeof(struct sockaddr_in);
	char welcome[MAXLINE + 1];

	// if (argc != 2) {
	// 	printf("사용법 : %s port\n", argv[0]);
	// 	exit(0);
	// }

	listen_sock = tcp_listen(INADDR_ANY, SERV_PORT, 5);
	if (listen_sock == -1)
		errquit("tcp_listen fail");
	if (set_nonblock(listen_sock) == -1)
		errquit("set_nonblock fail");
		
	puts("wait for client");
	for(count = 0; ; count++){
		if (count == 2500000) {
			puts("wait for client");
			fflush(stdout);
			count = 0;
		}
		accp_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &clilen);
		if (accp_sock <= 0 && errno != EWOULDBLOCK)
			errquit("accept fail");
		else if (accp_sock > 0){
			clisock_list[num_chat] = accp_sock;
			if (is_nonblock(accp_sock) != 0 && set_nonblock(accp_sock) < 0)
				errquit("set_nonblock fail");
			addClient(accp_sock, &cliaddr);
			send(accp_sock, START_STRING, strlen(START_STRING), 0);
			printf("%d번째 사용자 추가.\n", num_chat);
			sprintf(welcome, "%d번 사용자입니다.", num_chat);
			send(clisock_list[num_chat - 1], welcome, strlen(welcome), 0);
		}
		if(num_chat == 2)
				
			
		for (i = 0; i < num_chat; i++) {
			errno = 0;
			nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
			if (nbyte == 0) {
				removeClient(i);
				continue;
			}
			else if (nbyte == -1 && errno == EWOULDBLOCK)
				continue;
			if (strstr(buf, EXIT_STRING) != NULL) {
				removeClient(i);
				continue;
			}
			buf[nbyte] = 0;
			for (j = 0; j < num_chat; j++)
				send(clisock_list[j], buf, nbyte, 0);
			printf("%s\n", buf);
		}
	}
	return 0;
} //main


void recdvmsg()
{
	char readbuf;
	int abc=0;

	read(maxfd,&readbuf,strlen(readbuf));
	if(strstr(readbuf,"abc"))
	{
		abc++;
		if(abc==5)
		{
			//abc();
		}
		else
			write(maxfd,noabc,strlen(noabc));
	}
	else
		write(maxfd,&readbuf,strlen(readbuf));
}
	

		/*		//투표시작
					for (k = 0; k < num_chat; k++) {
					send(clisock_list[k], vote, strlen(vote), 0); // 투표하라는 명령
					if (gturn != 0)
						send(clisock_list[k], votenotice, strlen(votenotice), 0);   //죽은 사용자 알림
						
				}
							for (i = 0; i < num_chat; i++)
					nvote[i] = 0;
				while (1) {
						send(clisock_list[k], dieuser, strlen(dieuser), 0);
					int flag = 0;
					max = 0;

					if (indexx == 0) {
						printf("투표를 시작하겠습니다.\n\n");
						printf("게임 턴 횟수: %d\n\n", gturn);
					}

					for (i = 0; i < num_chat; i++) { // 너 투표해
						send(clisock_list[indexx], order, strlen(order), 0);
						for (k = 0; k < num_chat; k++) {
							if (k != indexx) {
								send(clisock_list[k], wait_so, strlen(wait_so), 0);
							}
						}
						while (1) {
							errno = 0;
							//nbyte = recv(clisock_list[i], vvote, MAXLINE, 0);
							nbyte = recv(clisock_list[indexx], vvote, MAXLINE, 0);
							if (strstr(vvote, EXIT_STRING) != NULL) {
								printf("비정상적인 종료입니다.\n");
								for (i = 0; i < num_chat; i++)
									send(clisock_list[i], vvote, strlen(vvote), 0);
								removeClient(i);
							}
							if (nbyte > 1) {
								indexx++; //
								break;   //while
							}
							if (nbyte == 0) {
								removeClient(i);
							}
							else if (nbyte == -1 && errno == EWOULDBLOCK)
								continue;

						}

						if (!strncmp(vvote, "1", 1))
							++nvote[0];
						else if (!strncmp(vvote, "2", 1))
							++nvote[1];
						else if (!strncmp(vvote, "3", 1))
							++nvote[2];
						else if (!strncmp(vvote, "4", 1))
							++nvote[3];
						else if (!strncmp(vvote, "5", 1))
							++nvote[4];
						for (i = 0; i < num_chat; i++)
							printf("%d  ", nvote[i]);
						printf("\n");
					}
					if (indexx == 5) {
						for (i = 0; i < num_chat; i++) {
							if (nvote[i] >= vtlimit[gturn]) {
								flag = 1;
								max = i + 1;
							}
						}
						if (flag == 1)
							break;
						else {
							for (i = 0; i < num_chat; i++)
								nvote[i] = 0;
							indexx = 0;
						}
					}

				}//while여기까지
				userid[turn] = max;       //죽은 사용자 번호
				turn++;
				printf("**마피아는 %d번입니다.**\n", mafia + 1);
				printf("**경찰은 %d번입니다.**\n\n", police + 1);
				printf("가장 많은 투표를 받은 사람은 %d번입니다\n", max);
				sprintf(votemsg, "%s %d번 입니다\n", voteresult, max);
				for (i = 0; i < num_chat; i++) {
					send(clisock_list[i], votemsg, strlen(votemsg), 0);
					if (i == max - 1)
						send(clisock_list[i], userdie, strlen(userdie), 0);
				}
				if (max == mafia + 1) { //마피아가 맞는 경우
					printf("%s\n 게임 종료\n", correct);
					for (i = 0; i < num_chat; i++) {
						send(clisock_list[i], correct, strlen(correct), 0);
						send(clisock_list[i], ctzvictory, strlen(ctzvictory), 0);
					}
					return;     //게임 종료
				}
				else {        //아닌 경우 (시민, 경찰)

					if (max == police + 1) {   //경찰이 죽음
						pflag = 1;
						printf("경찰이 죽었습니다.\n");
						for (i = 0; i < num_chat; i++)
							send(clisock_list[i], policedie, strlen(policedie), 0);//아니라는 말과 밤으로 넘어간다.
					}
					else {          //시민이 죽음
						numctz--;
						printf("\t남은 시민 수: %d명\n\n", numctz);
						if (pflag == 1 && numctz == 1) {
							for (i = 0; i < num_chat; i++)
								send(clisock_list[i], mafiavictory, strlen(mafiavictory), 0);
							return;
						}
						for (i = 0; i < num_chat; i++)
							send(clisock_list[i], incorrect, strlen(incorrect), 0);//아니라는 말과 밤으로 넘어간다.

					}
				}
				printf("밤이 되었습니다.\n\n");

				for (i = 0; i < num_chat; i++) {
					if (i == mafia) {   //마피아차례
						send(clisock_list[i], nightmsg, strlen(nightmsg), 0);//마피아 결정하라는 메시지
						while (1) {
							errno = 0;
							//nbyte = recv(clisock_list[i], vvote, MAXLINE, 0);
							nbyte = recv(clisock_list[mafia], target, MAXLINE, 0);//메세지 청취
							if (nbyte > 0) {
								indexx++; //
								break;   //while
							}
							if (nbyte == 0) {
								removeClient(i);
								continue;
							}
							else if (nbyte == -1 && errno == EWOULDBLOCK)
								continue;
							if (strstr(buf, EXIT_STRING) != NULL) {
								removeClient(i);
							}
						}

						userid[turn] = atoi(target);
						turn++;
						send(clisock_list[atoi(target) - 1], userdie, strlen(userdie), 0);
						if (atoi(target) - 1 == police) //마피아가 경찰을 죽이는 경우
						{
							pflag = 1;
							send(clisock_list[mafia], pcorrect_command, strlen(pcorrect_command), 0);
							for (i = 0; i < num_chat; i++)
								send(clisock_list[i], policedie, strlen(policedie), 0);
						}
						else { //시민을 죽이는경우
							send(clisock_list[mafia], pincorrect_command, strlen(pincorrect_command), 0);
							numctz--;
							printf("남은 시민 수: %d명\n", numctz);
							if (numctz == 1 && pflag == 0) {   // 시민 1명, 경찰 살아있는 경우
								for (i = 0; i < num_chat; i++) {
									if (i == police) {
										sprintf(police_nt, "\tuser%d번이 죽었습니다.\n", atoi(target));
										send(clisock_list[i], police_nt, strlen(police_nt), 0);
										send(clisock_list[i], police_command, strlen(police_command), 0);//경찰에게만

										while (1) {
											errno = 0;
											//nbyte = recv(clisock_list[i], vvote, MAXLINE, 0);
											nbyte = recv(clisock_list[police], target, MAXLINE, 0);//메세지 청취
											if (nbyte > 1) {

												indexx++; //
												break;   //while
											}
											if (nbyte == 0) {
												removeClient(i);
											}
											else if (nbyte == -1 && errno == EWOULDBLOCK)
												continue;
											if (strstr(buf, EXIT_STRING) != NULL) {
												removeClient(i);
											}
										}
										if (atoi(target) - 1 == mafia) {//경찰이 마피아를 맞출 경우
											send(clisock_list[police], victory, strlen(victory), 0);
											for (i = 0; i < num_chat; i++) {
												send(clisock_list[i], policevictory, strlen(policevictory), 0);
												send(clisock_list[i], ctzvictory, strlen(ctzvictory), 0);
											}
											return;    //마피아 잡고 게임 종료
										}
										else {     //  남은 시민 1명이 죽은 경우
											userid[turn] = atoi(target);
											for (i = 0; i < num_chat; i++) {
												send(clisock_list[i], policelose, strlen(policelose), 0);
												send(clisock_list[i], mafiavictory, strlen(mafiavictory), 0);
											}
											return;
										}
									}
								}
							}
							if (pflag == 1 && numctz == 1) {   //경찰이 죽고 시민 1명
								printf("시민의 수가 마피아의 수와 같습니다.\n★마피아의 승리★\n게임 종료\n");
								send(clisock_list[i], mafiavictory, strlen(mafiavictory), 0);
								return;
							}
						}
					}       //if mafia

				}//for
				gturn++;
				indexx = 0;//게임 턴
			}//두번째 while
		}  //게임을 시작합니다 if
	}// start
}
*/

void addClient(int s, struct sockaddr_in *newcliaddr) {
	char buf[20];

	inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
	printf("new client: %s\n", buf);
	clisock_list[num_chat] = s;
	num_chat++;

}
void removeClient(int s) {
	close(clisock_list[s]);
	if (s != num_chat - 1)
		clisock_list[s] = clisock_list[num_chat - 1];
	num_chat--;
	printf("채팅 참가자 1명 탈퇴 현재 참가자수 =%d\n", num_chat);
}
int getmax()
{
	int max = listen_sock;
	int i;
	for (i = 0; i < num_chat; i++)
		if (clisock_list[i] > max)
			max = clisock_list[i];
	return max;
}
int tcp_listen(int host, int port, int backlog) {
	int sd;
	struct sockaddr_in servaddr;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("socket fail");
		exit(1);
	}
	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(host);
	servaddr.sin_port = htons(port);
	if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind fail");
		exit(1);
	}
	listen(sd, backlog);
	return sd;
}
int set_nonblock(int sockfd) {
	int val;
	val = fcntl(sockfd, F_GETFL, 0);
	if (fcntl(sockfd, F_SETFL, val | O_NONBLOCK) == -1)
		return -1;
	return 0;
}
int is_nonblock(int sockfd) {
	int val;
	val = fcntl(sockfd, F_GETFL, 0);
	if (val & O_NONBLOCK)
		return 0;
	return -1;
}