/******************
***C语言贪吃蛇 ***
***作者:iimT*******
***2016/12/12****
*******************/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

const int W = 52;//地图宽度
const int H  = 22;//地图高度

char map[22][52];//定义地图
char key;//用来保存用户操作时输入的键值

int direct = 4;//蛇当前的运动方向
int food[2] = {9,11};//food[0]食物横坐标  food[1]食物纵坐标
int head;//记录长度

int snake[400][3];//蛇的所有坐标点以及蛇   身体(snake[i][0]=0)   和    蛇头(snake[i][0]=1)   从尾部向头部走

void init(char map[22][52], int snake[400][3]);//游戏开始的时候初始化
void makeMap(char map[22][52],int snake[400][3],int food[]);//根据move后的蛇和生成的food更改map里面的值---制作地图
void move(int snake[400][3],int direct);//让蛇的坐标进行移动
void makeFood(int food[]);//生成随机数制作一个食物坐标
void showView(char map[22][52]);  //绘出视图
int ifEat(int head, int food[2]); //判断食物是否被吃掉
int ifReprat(int snake[400][3], int x, int y);//判断生成的食物是否与蛇有重复
int ifBump(int head);//判断是否会相撞(撞自己或撞墙)
void getKey();//读取键值并重新设置运动方向


int main() {
	init(map, snake);//初始化地图
	while (1) {
		getKey();
		system("cls");
		Sleep(30);
		move(snake, direct);//让蛇开始移动
		if (!food[0]&&!food[1]) {//食物已经被吃掉--重新生成食物
			makeFood(food);//生成食物坐标
		}
		makeMap(map, snake, food);
		showView(map);
		if (ifBump(head)) {
			printf("游戏结束，你的成绩为：%d", head);
			break;
		}
		getKey();
	}
	getchar();
	return 0;
}

void init(char map[22][52], int snake[400][3]) {//初始化
	//初始化蛇
	snake[0][0] = 0, snake[0][1] = 9, snake[0][2] = 7;
	snake[1][0] = 0, snake[1][1] = 9, snake[1][2] = 8;
	snake[2][0] = 1, snake[2][1] = 9, snake[2][2] = 9;//蛇头
	//初始化地图
	for (int i = 0;i<H;i++) {
		for (int j = 0;j<W;j++) {
			if (i == 0 || j == 0 || i == H - 1 || j == W - 1) {
				map[i][j] = '*';
			}
			else {
				map[i][j] = ' ';
			}
		}
	}
}

void showView(char map[22][52]) {//根据map数组中存的值打印出来
	for (int i = 0;i<H;i++) {
		for (int j = 0;j<W;j++) {
				printf("%c", map[i][j]);
		}
		printf("\n");
	}
}

void move(int snake[400][3],int direct) {//让蛇的坐标开始移动
	int x, y;//作为临时交换的值  head为蛇头
	for (int i = 0;i < 400;i++) {
		if (snake[i][0] == 1) {
			head = i;
			break;
		}
	}
	//找到头并将头的坐标保留用于下一个补上来
	x = snake[head][1];
	y = snake[head][2];
	switch (direct){
		case 1://向上
			snake[head][1]--;//横坐标不变，纵坐标+1
			break;
		case 2://向下
			snake[head][1]++;//横坐标不变，总左边-1
			break;
		case 3://向左
			snake[head][2]--;//纵坐标不变，横坐标-1
			break;
		case 4://向右
			snake[head][2]++;//纵坐标不变，横坐标+1
			break;
	}
	//头已经到了下一个位置，先判断是否会吃掉食物，接下来将之前所有的身体放到前一个的位置上
	if (ifEat(head, food)) {
		snake[head + 1][0] = 1, snake[head + 1][1] = food[0], snake[head + 1][2] = food[1];//重新定义蛇头
		snake[head][0] = 0;//取消之前的蛇头
		food[0] = 0, food[1] = 0;//将当前的food置空
	}
		for (int j = head - 1;j >= 0;j--) {
			int temp;
			temp = x, x = snake[j][1], snake[j][1] = temp;
			temp = y, y = snake[j][2], snake[j][2] = temp;
		}
}

void makeFood(int food[]) {//生成一个食物的坐标
	srand(time(0));
	int x = rand() % 49 + 2, y = rand() % 19 + 2;//生成一个1-50的数作为横坐标  1-20的数作为纵坐标
	while (ifReprat(snake,x,y)) {//如果有重复就重新生成一个  直到不重复为止
		x = rand() % 49 + 2, y = rand() % 19 + 2;
	}
	food[0] = y;
	food[1] = x;
}

void makeMap(char map[22][52], int snake[400][3], int food[]) {//根据蛇的坐标  食物的坐标重置地图的值
	int i;
	//重新初始化地图
	for (int i = 0;i<H;i++) {
		for (int j = 0;j<W;j++) {
			if (i == 0 || j == 0 || i == H - 1 || j == W - 1) {
				map[i][j] = '*';
			}
			else {
				map[i][j] = ' ';
			}
		}
	}
	//重新绘制蛇
	for (i = 0;i < 400;i++) {
		if (snake[i][0] == 1) break;
		map[snake[i][1]][snake[i][2]] = '#';
	}
	//绘制头和食物
	map[snake[i][1]][snake[i][2]] = '@';
	map[food[0]][food[1]] = 'o';
}


int ifEat(int head,int food[2]) {
	if (snake[head][1] == food[0] && snake[head][2] == food[1])
		return 1;
	else
		return 0;
}

int ifReprat(int snake[400][3],int x,int y) {//判断生成的食物是否与蛇有重复
	for (int i = 0;i < 400;i++) {
		if (snake[i][0] == 1) break;
		if ((snake[i][1] == x&&snake[i][2] == y)) {//有重复
			return 1;
		}
	}
	return 0;
}

int ifBump(int head) {//判断是否会相撞(撞自己或撞墙)
		if ((snake[head][2]==0|| snake[head][2] == 51)  ||  (snake[head][1] == 0|| snake[head][1] == 21))//撞墙
			return 1;
		for (int i = 0;i < head-1;i++) {
			if ((snake[i][1] == snake[head][1]&&snake[i][2] == snake[head][2])) {//幢自己
				return 1;
			}
		}
		return 0;
}
void getKey() {
	if (_kbhit()) {
		key = _getch();
	}
	switch (key) {
	case 'H'://用户按 上 键
		if (direct != 2)
			direct = 1;
		break;
	case 'P'://用户按 下 键
		if (direct != 1)
			direct = 2;
		break;
	case 'K'://用户按 左 键
		if (direct != 4)
			direct = 3;
		break;
	case 'M'://用户按 右 键
		if (direct != 3)
			direct = 4;
		break;
	}
}