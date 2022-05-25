#include <bangtal>
using namespace bangtal;
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define ANIMATION_TIME		0.1f	//�����̴� �ð� ����
#define ANIMATION_STEP		150	//�����Ҷ� �̵��ϴ� �߰��� y��ǥ
#define DELAY_TIME 0.2f	//����Ű:������ ó�� �ð� ����

SceneID start_scene, main_scene, end_scene;
ObjectID playscene; //���ȭ�� ������Ʈ
ObjectID cat, silver, gold;
int coin_silverX = 300, coin_silverY = 300;
int coin_goldX = 500, coin_goldY = 500;
int coin_count;

TimerID timerAni = createTimer(ANIMATION_TIME);
TimerID timerRelease = createTimer(DELAY_TIME);	//����Ű ������ ó�� Ÿ�̸�
TimerID sceneTimer = createTimer(10000);

int x = 200, y = 50;	//�ʱ� ��ü ��ǥ
int dx = 0, dy = 0;		//��ü �̵� ��ǥ ����

KeyState keyState = KeyState::KEY_RELEASED;	//Ű���� ���� ����

void keyboardCallback(KeyCode code, KeyState state);	//Ű���� �ݹ� �Լ�
void timerCallback(TimerID timer);	//Ÿ�̸� �ݹ� �Լ�

//��ֹ�
ObjectID ob[4];
int ob_x[4]; //��ֹ��� x��ǥ

int ob_dx = 0, speed = 20;
TimerID ob_speed_timer; //������ ����ɼ��� ��ֹ��� �̵��ϴ� �ӵ��� �������� �����ϴ� Ÿ�̸�
TimerID ob_timer; //��ֹ� ��ġ, ��ֹ��� �̵��ϵ��� �ϴ� Ÿ�̸�

//������Ʈ �����ϴ� �Լ�
ObjectID createObject(const char* name, const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	showObject(object);

	if (shown) {
		showObject(object);
	}

	return object;
}

//ĳ���Ͱ� ���ο� �ε������� üũ�ϴ� �Լ�.
//�ε����� ������ ������鼭 ������ ����
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

void keyboardCallback(KeyCode code, KeyState state)	//����Ű ���������� ���� ����Ű:������ ����->Ÿ�̸ӿ��� ó��
{
	if (code == KeyCode::KEY_SPACE && state == KeyState::KEY_PRESSED) {
		if (y > 50 + ANIMATION_STEP) return;	//2�� ������ �����ϰ� �����
		y += ANIMATION_STEP;
		locateObject(cat, main_scene, x, y);
		stopTimer(timerRelease);	//������ ����� Ÿ�̸� ���
		setTimer(timerRelease, DELAY_TIME);	//����Ű ������ Ÿ�̸� �ٽ� ����
		startTimer(timerRelease);	//Ÿ�̸� ����
	}
}

void timerCallback(TimerID timer)
{
	if (timer == timerRelease)
	{
		y = 50;
		locateObject(cat, main_scene, x, y);	//�����̴� ��ü �ٽ� ���ƿ��� �����
	}
	if (timer == ob_timer) {
		for (int i = 0; i < 4; i++) {
			hideObject(ob[i]);//�ʿ��������
			if (ob_x[i] >= -100) {
				ob_x[i] -= speed;
				locateObject(ob[i], main_scene, ob_x[i], 0);
				showObject(ob[i]);//�ʿ��������
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

	playscene = createObject("playscene.png");	//���ȭ��
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