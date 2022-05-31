#include <bangtal>
using namespace bangtal;
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define ANIMATION_TIME		0.1f	//�����̴� �ð� ����
#define ANIMATION_STEP		150	//�����Ҷ� �̵��ϴ� �߰��� y��ǥ
#define DELAY_TIME 0.2f	//����Ű:������ ó�� �ð� ����

SceneID start_scene, main_scene, end_scene;
ObjectID cat, silver, gold; //�����̴� ������Ʈ(�����), ���� ����, �ݻ� ���� ����

ObjectID bar;	//�Ÿ� �� ������Ʈ ����
ObjectID barCh;	//�ٸ� �����̴� ĳ���� ����

int coin_silverX = 300, coin_silverY = 300; //���� ���� ����
int coin_goldX = 500, coin_goldY = 500; //�ݻ� ���� ����
int coin_count; //����

double barCh_x = 320;	//�ٸ� �����̴� ĳ������ �ʱ� ��ǥ ����

TimerID timerRelease = createTimer(DELAY_TIME);	//����Ű ������ ó�� Ÿ�̸�(���� �ð�)
TimerID sceneTimer = createTimer(10000); //���� �� �ð� ���� Ÿ�̸�
TimerID barTimer = createTimer(0.01f);	//�ٸ� �����̴� ĳ���� Ÿ�̸� ����

int x = 200, y = 50;	//�ʱ� ��ü ��ǥ
int dx = 0, dy = 0;		//��ü �̵� ��ǥ ����

KeyState keyState = KeyState::KEY_RELEASED;	//Ű���� ���� ����

void keyboardCallback(KeyCode code, KeyState state);	//Ű���� �ݹ� �Լ�
void timerCallback(TimerID timer);	//Ÿ�̸� �ݹ� �Լ�

//��ֹ�
ObjectID ob[4];
int ob_x[4]; //��ֹ��� x��ǥ
int ob_dx = 0, speed = 7;
int ob_check[4];//��ֹ��� �̹����� Ȯ���ϱ� ���� ����

TimerID ob_speed_timer; //������ ����ɼ��� ��ֹ��� �̵��ϴ� �ӵ��� �������� �����ϴ� Ÿ�̸�
TimerID ob_timer; //��ֹ� ��ġ, ��ֹ��� �̵��ϵ��� �ϴ� Ÿ�̸�
TimerID check_timer; //��ֹ��� ��Ű�� �浹 Ȯ���ϴ� Ÿ�̸�

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

	if (timer == barTimer) {	//�ٸ� �����̴� �Լ�
		hideObject(barCh);
		barCh_x = barCh_x + 0.076;
		locateObject(barCh, main_scene, barCh_x, 680);
		showObject(barCh);
	}

	setTimer(barTimer, 0.01f);	//�ٸ� �����̴� �ð� ����
	startTimer(barTimer);	//�� Ÿ�̸� ����

	if (timer == ob_timer) {
		for (int i = 0; i < 4; i++) {
			hideObject(ob[i]);//�ʿ��������
			if (ob_x[i] >= -100) {
				ob_x[i] -= speed;
				locateObject(ob[i], main_scene, ob_x[i], 50);
				showObject(ob[i]);//�ʿ��������
			}
			else if (ob_x[i] < -100) {
				if (i == 0) {
					ob_x[i] = 1800;
					int n = rand() % 2;
					switch (n) {
					case 0: setObjectImage(ob[i], "images\\ob_low.png"); ob_check[i] = 1; break;
					case 1: setObjectImage(ob[i], "images\\ob_high.png"); ob_check[i] = 2; break;
					}
					break;
					locateObject(ob[i], main_scene, ob_x[i], 50);
					showObject(ob[i]);

				}


				else if (i == 1) {
					int n = rand() % 2;
					switch (n) {
					case 0: ob_x[i] = ob_x[0] + 400 + ob_dx, setObjectImage(ob[i], "images\\ob_low.png"); ob_check[i] = 1; break;
					case 1: ob_x[i] = ob_x[0] + 500 + ob_dx, setObjectImage(ob[i], "images\\ob_high.png"); ob_check[i] = 2; break;
					}
					locateObject(ob[i], main_scene, ob_x[i], 50);
					showObject(ob[i]);
				}

				else if (i == 2) {
					int n = rand() % 2;
					switch (n) {
					case 0: ob_x[i] = ob_x[1] + 400 + ob_dx, setObjectImage(ob[i], "images\\ob_low.png"); ob_check[i] = 1; break;
					case 1: ob_x[i] = ob_x[1] + 500 + ob_dx, setObjectImage(ob[i], "images\\ob_high.png"); ob_check[i] = 2; break;
					}
					locateObject(ob[i], main_scene, ob_x[i], 50);
					showObject(ob[i]);

				}
				else if (i == 3) {
					int n = rand() % 2;
					switch (n) {
					case 0: ob_x[i] = ob_x[2] + 400 + ob_dx, setObjectImage(ob[i], "images\\ob_low.png"); ob_check[i] = 1; break;
					case 1: ob_x[i] = ob_x[2] + 500 + ob_dx, setObjectImage(ob[i], "images\\ob_high.png"); ob_check[i] = 2; break;
					}
					locateObject(ob[i], main_scene, ob_x[i], 50);
					showObject(ob[i]);

				}
			}

		}
		setTimer(ob_timer, 0.01f);
		startTimer(ob_timer);
	}
	if (timer == ob_speed_timer) {
		if (speed <= 50) {
			ob_dx += 1;
			speed += 1;
			setTimer(ob_speed_timer, 5.0f);

		}
	}

	if (timer == check_timer) {   //0.01�ʸ��� �浹 Ȯ��, check number�� �̿��ؼ� ��ֹ��� �̹����� ����, ��Ű ������ �� �߰� ��ġ������ ��� y = 50, 200�� �������� �浹 Ȯ����.

		for (int i = 0; i < 4; i++) {
			if (ob_x[i] < 271 && ob_x[i]> 94) {
				if (ob_check[i] == 1) {  // ���� ��ֹ��� ���
					if (y == 50) {
						endGame();
					}
				}
				else if (ob_check[i] == 2) {
					if (y == 50 || y == 200) {  //���� ��ֹ��� ���
						endGame();
					}
				}
			}
		}
		setTimer(check_timer, 0.01f);
		startTimer(check_timer);
	}
}

int main() {
	//setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	start_scene = createScene("���� ȭ��", "images\\main_scene.png");
	main_scene = createScene("���� ȭ��", "images\\main_scene.png");
	end_scene = createScene("���� ȭ��", "images\\main_scene.png");

	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);

	cat = createObject("cat", "images\\cat.png", main_scene, x, y, true);
	silver = createObject("silver", "images\\silver.png", main_scene, coin_silverX, coin_silverY, true);
	gold = createObject("gold", "images\\gold.png", main_scene, coin_goldX, coin_goldY, true);

	bar = createObject("images/bar.png");	//��
	locateObject(bar, main_scene, 320, 680);
	showObject(bar);
	barCh = createObject("images/cat.png");	//�ٸ� �����̴� ĳ����
	scaleObject(barCh, 0.25f);
	locateObject(barCh, main_scene, 320, 680);
	showObject(barCh);

	CoinCheck();

	ob_speed_timer = createTimer(5.0f);
	ob_timer = createTimer(0.01f);
	check_timer = createTimer(0.01f);

	for (int i = 0; i < 4; i++) {
		ob_x[i] = 500 + 500 * i;
		ob_check[i] = 1;
		ob[i] = createObject("ob_block", "images\\ob_low.png", main_scene, ob_x[i], 50, true);
	}

	startTimer(ob_timer);
	startTimer(check_timer);
	startTimer(ob_speed_timer);
	startTimer(barTimer);

	//startGame(start_scene);
	startGame(main_scene);
}