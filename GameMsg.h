#ifndef __GAMEMSG_H__
#define __GAMEMSG_H__

#define SERV_PORT 3305

#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* server msg */

char *EXIT_STRING = "exit";
char *START_STRING = "방에 입장하였습니다. 게임 대기중입니다..\n";
char *start_command1 = "start";
char *start_command2 = "\nGAME START\n";
char *order = "your turn. choice number. ";
char *wait_so = "wait..\n";
char *interrupt = "비정상적인 종료입니다. 게임을 다시 시작하세요.\n";
char *freetime = "투표를 시작하실려면 투표라고 입력해주세요";
char *noabc ="아직 투표인원수가 충족되지 않았습니다\n";


/* ingame msg */
char *policedie = "\t경찰이 죽었습니다.\n\t*******조심하세요!********\n";
char *policevictory = "\t★똑똑한 경찰이 마피아를 잡았습니다★\n\n";
char *policelose = "\t멍청한 경찰이 마피아를 못잡았습니다.\n";

char *correct = "\t그는 마피아가 맞습니다!\n\n";
char *incorrect = "\t선량한 시민이 죽었습니다.\n\n\t밤이 되었습니다.\n";
char *pcorrect_command = "\t경찰이 죽었습니다. 시민 수와 동일해질때까지 살아남으세요\n";
char *pincorrect_command = "\t시민이 죽었습니다.\n\n";
char *police_command = "\t경찰은 마피아로 의심되는 사람을 지목하세요\n";

char *mfmsg = "당신은 마피아입니다.\n";
char *plcmsg = "당신은 경찰입니다. 밤에 마피아가 누군지 알수 있는 기회가 부여됩니다.\n";
char *ctzmsg = "당신은 시민입니다. 마피아를 찾아주세요\n";
char *morning = "\n\t낮이 밝았습니다. 마피아가 누구일지 결정해주세요\n";
char *nightmsg = "\t누구를 죽일 것인지 정해주세요: ";
char *vote = "------------------------------------------------------\n\t\t투표를 시작합니다.\n\t마피아라고 생각하는 사용자의 번호를 입력하세요\n";
char *voteresult = "------------------------------------------------------\n\t가장 많은 투표를 받은 사람은";
char *votenotice = "다음 사용자들을 제외하고 투표하세요)";
char *userdie = "\t사용자는 죽었습니다..투표에서 0을 눌러주세요\n";
char *victory = "\t그는 마피아입니다.\n\n";
char *nvictory = "\t그는 마피아가 아닙니다.\n";
char *mafiavictory = "\t★★★마피아의 승리입니다★★★\n";
char *ctzvictory = "\t★★★시민의 승리입니다★★★\n";

#endif
