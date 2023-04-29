#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

//�������� M ���� N ���
const int M = 20;
const int N = 10;

int field[M][N] = { 0 };		//�Ѿ����µķ���

//����ṹ��
struct Point
{
	int x, y;
} a[4], b[4];
//�������͵�����
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

//����Ƿ񳬹��߿� �� �Ƿ������Ѿ����µķ���(field)
bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y > M)
			return 0;
		else if (field[a[i].y][a[i].x])
			return 0;
	return 1;
}
//ִ����Ϸ
void play()
{
	//��������
	RenderWindow window(VideoMode(360, 540), "SFML tinytetris");

	//�������ط���ͼƬ
	Texture t;
	t.loadFromFile("images/tiles.png");

	Sprite s(t);
	s.setTextureRect(IntRect(0, 0, 18, 18));

	//�������ر���ͼƬ
	Texture bg;
	bg.loadFromFile("images/background.png");

	Sprite background(bg);
	background.setTextureRect(IntRect(0, 0, 350, 510));

	//�������ر߿�ͼƬ
	Texture f;
	f.loadFromFile("images/frame.png");

	Sprite frame(f);
	frame.setTextureRect(IntRect(0, 0, 320, 480));

	int dx = 0;		//���ݰ����������ƶ���ֵ
	bool rotate = 0;		//��ת
	int colorNum = 1;
	//timer �ܵ�ʱ�䣬delay ���ִ��һ�ε�ʱ��
	float timer = 0, delay = 0.3;

	Clock clock;
	
	//��ʼ�� ����ͼ��
	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}

	//�����򿪴��ڣ��������һֱ���ŵĻ�
	while (window.isOpen() == true)
	{
		float time = clock.getElapsedTime().asSeconds();		//��ǰ���ϴε���restart()������ʱ��
		clock.restart();		//������ʱ
		timer += time;		//�ۼ�ʱ��
		//��ǰ�Ĵ����¼���ֵ���¼�����event
		Event event;
		while (window.pollEvent(event))
		{
			//�¼�Ϊ�رգ��رմ���
			if (event.type == Event::Closed)
				window.close();
			//�¼�����ʱ ��Ϊ��ת����-1 �������꣬��+1��������
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
		//���� �� ���������ƶ�
		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

		//�ƶ�
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (!check())
			for (int i = 0; i < 4; i++)
				a[i] = b[i];

		//��ת ˳ʱ��
		if (rotate)
		{
			Point p = a[1];		//���±�Ϊ1�ķ���Ϊ���Ľ�����ת
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
		//�ۼƵ�ʱ��timer ���� delay  ִ�������ƶ�
		if (timer > delay)
		{
			cout << "------" << endl;
			//��ʱ ���������ƶ�
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!check())
			{
				//���Ѿ�����ײ���Ե�ķ��飬��������
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;
				n = rand() % 7;		//����ͼ�ε����� �������
				//�����µķ���ͼ��
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}
			timer = 0;
		}

		//���µ��� ÿ�еļ��
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;		//ÿ�еķ�������
			for (int j = 0; j < N; j++)
			{
				if (field[i][j])
					count++;
				/*���и�ֵ���������i == k������������һ��ʱ��
				k������--������һ��ѭ����k����֮ǰ��������
				��ô��ֵ�ͻ��� ��ǰ�и���֮ǰ�е�����*/
				field[k][j] = field[i][j];
			}
			if (count < N)
				k--;
		}

		//��ʼ�� �ƶ�����ת���ӳ�
		dx = rotate = 0;
		delay = 1;

		window.clear(Color::White);//������
		window.draw(background);
		//����������ķ������ݣ�����������
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0)
					continue;
				//�������ʱ����� colorNum�����÷���ͼƬ(��ȡͼƬ)
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18, i * 18);
				s.move(28, 31);
				window.draw(s);//�����ص�ͼƬ
			}

		//���ݷ�������*18(��������ͼƬ�Ŀ��),���÷����ͼƬ����
		for (int i = 0; i < 4; i++)
		{
			//���÷���ͼƬ
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a[i].x * 18, a[i].y * 18);
			s.move(28, 31);
			window.draw(s);//�����ص�ͼƬ
		}
		window.draw(frame);
		window.display();//��ʾ
	};
}

int main()
{
	play();
	return 0;
}