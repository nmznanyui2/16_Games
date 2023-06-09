#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

//格子数量 M 竖格 N 横格
const int M = 20;
const int N = 10;

int field[M][N] = { 0 };		//已经落下的方块

//坐标结构体
struct Point
{
	int x, y;
} a[4], b[4];
//方块类型的数组
int figures[7][4] =
{
	{0, 2, 4, 6},		//I
	{1, 2, 3, 4},		//Z
	{0, 2, 3, 5},		//S
	{0, 2, 3, 4},		//T
	{0, 2, 4, 5},		//L
	{1, 3, 4, 5},		//J
	{0, 1, 2, 3},		//Z
};

//检查是否超过边框 或 是否碰到已经落下的方块(field)
bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y > M)
			return 0;
		else if (field[a[i].y][a[i].x])
			return 0;
	return 1;
}
//执行游戏
void play()
{
	//创建窗口
	RenderWindow window(VideoMode(360, 540), "SFML tinytetris");

	//创建加载方块图片
	Texture t;
	t.loadFromFile("images/tiles.png");

	Sprite s(t);
	s.setTextureRect(IntRect(0, 0, 18, 18));

	//创建加载背景图片
	Texture bg;
	bg.loadFromFile("images/background.png");

	Sprite background(bg);
	background.setTextureRect(IntRect(0, 0, 350, 510));

	//创建加载边框图片
	Texture f;
	f.loadFromFile("images/frame.png");

	Sprite frame(f);
	frame.setTextureRect(IntRect(0, 0, 320, 480));

	int dx = 0;		//根据按键操作，移动的值
	bool rotate = 0;		//翻转
	int colorNum = 1;
	//timer 总的时间，delay 多久执行一次的时间
	float timer = 0, delay = 0.3;

	Clock clock;
	
	//初始化 方块图形
	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}

	//持续打开窗口，如果窗口一直开着的话
	while (window.isOpen() == true)
	{
		float time = clock.getElapsedTime().asSeconds();		//当前距上次调用restart()已流逝时间
		clock.restart();		//启动计时
		timer += time;		//累计时间
		//当前的窗口事件赋值给事件变量event
		Event event;
		while (window.pollEvent(event))
		{
			//事件为关闭，关闭窗口
			if (event.type == Event::Closed)
				window.close();
			//事件按键时 上为翻转，左-1 方块坐标，右+1方块坐标
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Up)
					rotate = true;
				else if (event.key.code == Keyboard::Left)
					dx = -1;
				else if (event.key.code == Keyboard::Right)
					dx = 1;
			}
		}
		//按键 下 加速向下移动
		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

		//移动
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (!check())
			for (int i = 0; i < 4; i++)
				a[i] = b[i];

		//翻转 顺时针
		if (rotate)
		{
			Point p = a[1];		//以下标为1的方块为中心进行旋转
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
		}
		//累计的时间timer 大于 delay  执行向下移动
		if (timer > delay)
		{
			cout << "------" << endl;
			//定时 方块向下移动
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!check())
			{
				//把已经到达底部边缘的方块，存入数组
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				n = rand() % 7;		//方块图形的类型 进行随机
				//生成新的方块图形
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}
			timer = 0;
		}

		//从下到上 每行的检查
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;		//每行的方块数量
			for (int j = 0; j < N; j++)
			{
				if (field[i][j])
					count++;
				/*进行赋值，正常情况i == k，但当方块满一行时，
				k不进行--，再下一次循环，k还是之前的行数，
				那么赋值就会变成 当前行覆盖之前行的数据*/
				field[k][j] = field[i][j];
			}
			if (count < N)
				k--;
		}

		//初始化 移动，翻转，延迟
		dx = rotate = 0;
		delay = 1;

		window.clear(Color::White);//清理窗口
		window.draw(background);
		//查找数组里的方块数据，画到界面上
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0)
					continue;
				//根据落地时保存的 colorNum，设置方块图片(截取图片)
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				s.move(28, 31);
				window.draw(s);//画加载的图片
			}

		//根据方块坐标*18(单个方块图片的宽度),设置方块的图片坐标
		for (int i = 0; i < 4; i++)
		{
			//设置方块图片
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			s.move(28, 31);
			window.draw(s);//画加载的图片
		}
		window.draw(frame);
		window.display();//显示
	};
}

int main()
{
	play();
	return 0;
}