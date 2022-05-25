#include <bangtal>
using namespace bangtal;
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define ANIMATION_TIME		0.1f	//움직이는 시간 정의
#define ANIMATION_STEP		150	//점프할때 이동하는 발걸음 y좌표
#define DELAY_TIME 0.2f	//점프키:릴리즈 처리 시간 정의

SceneID start_scene, main_scene, end_scene;
ObjectID playscene; //배경화면 오브젝트
ObjectID cat, silver, gold;
int coin_silverX = 300, coin_silverY = 300;
int coin_goldX = 500, coin_goldY = 500;
int coin_count;

TimerID timerAni = createTimer(ANIMATION_TIME);
TimerID timerRelease = createTimer(DELAY_TIME);	//점프키 릴리즈 처리 타이머
TimerID sceneTimer = createTimer(10000);

int x = 200, y = 50;	//초기 물체 좌표
int dx = 0, dy = 0;		//물체 이동 좌표 정의

KeyState keyState = KeyState::KEY_RELEASED;	//키보드 상태 정의

void keyboardCallback(KeyCode code, KeyState state);	//키보드 콜백 함수
void timerCallback(TimerID timer);	//타이머 콜백 함수

//장애물
ObjectID ob[4];
int ob_x[4]; //장애물의 x좌표

int ob_dx = 0, speed = 20;
TimerID ob_speed_timer; //게임이 진행될수록 장애물이 이동하는 속도가 빨라지게 조절하는 타이머
TimerID ob_timer; //장애물 배치, 장애물이 이동하도록 하는 타이머

//오브젝트 선언하는 함수
ObjectID createObject(const char* name, const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	showObject(object);

	if (shown) {
		showObject(object);
	}

	return object;
}

//캐릭터가 코인에 부딪혔는지 체크하는 함수.
//부딪히면 코인이 사라지면서 점수가 증가
void CoinCheck() {
	if (coin_silverX < 100 + 576 && coin_silverX > 100 && coin_silverY > 100 && coin_silverY < 100 + 324) {
		hideObject(silver);
		coin_count += 1;
	}

	if (coin_goldX < 100 + 576 && coin_goldX > 100 && coin_goldY > 100 && coin_goldY < 100 + 324) {
		hideObject(gold);
		coin_count += 10;
	}
}

void keyboardCallback(KeyCode code, KeyState state)	//점프키 이중점프를 위해 점프키:릴리즈 상태->타이머에서 처리
{
	if (code == KeyCode::KEY_SPACE && state == KeyState::KEY_PRESSED) {
		if (y > 50 + ANIMATION_STEP) return;	//2단 점프만 가능하게 만들기
		y += ANIMATION_STEP;
		locateObject(cat, main_scene, x, y);
		stopTimer(timerRelease);	//이전에 실행된 타이머 취소
		setTimer(timerRelease, DELAY_TIME);	//점프키 릴리즈 타이머 다시 실행
		startTimer(timerRelease);	//타이머 시작
	}
}

void timerCallback(TimerID timer)
{
	if (timer == timerRelease)
	{
		y = 50;
		locateObject(cat, main_scene, x, y);	//움직이는 물체 다시 돌아오게 만들기
	}
	if (timer == ob_timer) {
		for (int i = 0; i < 4; i++) {
			hideObject(ob[i]);//필요없을지도
			if (ob_x[i] >= -100) {
				ob_x[i] -= speed;
				locateObject(ob[i], main_scene, ob_x[i], 0);
				showObject(ob[i]);//필요없을지도
			}
			else if (ob_x[i] < -100) {
				startTimer(ob_speed_timer);

				if (i == 0) {
					ob_x[i] = 1800;
					int n = rand() % 3;
					switch (n) {
					case 0: setObjectImage(ob[i], "gift.jpg"); break;
					case 1: setObjectImage(ob[i], "end.png"); break;
					case 2: setObjectImage(ob[i], "tree.png"); break;
					}
					break;
					locateObject(ob[i], main_scene, ob_x[i], 0);
					showObject(ob[i]);

				}


				else if (i == 1) {
					int n = rand() % 3;
					switch (n) {
					case 0: ob_x[i] = ob_x[0] + 400 + ob_dx, setObjectImage(ob[i], "gift.jpg"); break;
					case 1: ob_x[i] = ob_x[0] + 450 + ob_dx, setObjectImage(ob[i], "end.png"); break;
					case 2: ob_x[i] = ob_x[0] + 500 + ob_dx, setObjectImage(ob[i], "tree.png"); break;
					}
					locateObject(ob[i], main_scene, ob_x[i], 0);
					showObject(ob[i]);
				}

				else if (i == 2) {
					int n = rand() % 3;
					switch (n) {
					case 0: ob_x[i] = ob_x[1] + 400 + ob_dx, setObjectImage(ob[i], "gift.jpg"); break;
					case 1: ob_x[i] = ob_x[1] + 450 + ob_dx, setObjectImage(ob[i], "tree.png"); break;
					case 2: ob_x[i] = ob_x[1] + 500 + ob_dx, setObjectImage(ob[i], "end.png"); break;
					}
					locateObject(ob[i], main_scene, ob_x[i], 0);
					showObject(ob[i]);

				}
				else if (i == 3) {
					int n = rand() % 3;
					switch (n) {
					case 0: ob_x[i] = ob_x[2] + 400 + ob_dx, setObjectImage(ob[i], "gift.jpg"); break;
					case 1: ob_x[i] = ob_x[2] + 450 + ob_dx, setObjectImage(ob[i], "tree.png"); break;
					case 2: ob_x[i] = ob_x[2] + 500 + ob_dx, setObjectImage(ob[i], "end.png"); break;
					}
					locateObject(ob[i], main_scene, ob_x[i], 0);
					showObject(ob[i]);

				}
			}

		}
		setTimer(ob_timer, 0.1f);
		startTimer(ob_timer);
	}
	if (timer == ob_speed_timer) {
		if (speed <= 50) {
			x += 2;
			speed += 2;
			setTimer(ob_speed_timer, 0.1f);

		}
	}
}

int main() {
	//setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	main_scene = createScene("main_scene", "images\\Background.png");

	playscene = createObject("playscene.png");	//배경화면
	locateObject(playscene, main_scene, 0, 0);
	showObject(playscene);

	cat = createObject("cat", "images\\cat.png", main_scene, x, y, true);
	silver = createObject("silver", "images\\silver.png", main_scene, coin_silverX, coin_silverY, true);
	gold = createObject("gold", "images\\gold.png", main_scene, coin_goldX, coin_goldY, true);

	CoinCheck();

	ob_speed_timer = createTimer(0.1f);
	ob_timer = createTimer(0.01f);

	for (int i = 0; i < 4; i++) {
		ob_x[i] = 500 + 500 * i;
		;		ob[i] = createObject("ob_block","images\\gift.png", main_scene, ob_x[i], 0, true);
	}

	startTimer(ob_timer);
	startTimer(timerAni);
	startGame(start_scene);
}