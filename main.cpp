#include "Tetris.h"

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{
	int x, y;
}a[4], b[4];

int figures[7][4] = { {1,3,5,7}, {2,4,5,7}, {3,5,4,6}, {3,5,4,7}, {2,3,5,7}, {3,5,7,6}, {2,3,4,5} };
//                        I          Z          S          T          L          J          O

//This function will check if shape out of edge, if so return 0, else return 1.
bool check()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;
	}

	return 1;
}

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(330, 660), "Tetris Game!");//Width: 33 * N , Height: 33 * M
	Texture t;
	t.loadFromFile("block.png");

	Sprite s(t);
	s.scale(0.05f, 0.05f);//To get a fit size in window.
	s.setTextureRect(IntRect(0, 0, 700, 1000));//when 700, 1000, graph is suqare.

	int dx = 0, colorNum = 1; 
	bool rotate = 0;

	float timer = 0, delay = 0.3;

	Clock clock;


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();//return passed time as secons to time and set start time to 0 again.
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
			
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Up) rotate = true;//When player press up, shape will rotate.
				else if (e.key.code == Keyboard::Left) dx = -1;//Press left make shape left move.
				else if (e.key.code == Keyboard::Right) dx = 1;//Press right make shape right move.
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;//Speed up down.

		//Move
		for (int i = 0; i < 4; i++) 
		{
			b[i] = a[i];
			a[i].x += dx;//If not left or right, would move to left or right.
		}
		if (!check())
		{
			for (int i = 0; i < 4; i++)
			{
				a[i] = b[i];
			}
		}

		//Rotate
		if (rotate)
		{
			Point p = a[1]; // Let a[1] be point center.
			for (int i = 0; i < 4; i++)//This part make shape rotate, actully using fomular of rotation in coordinate system
			{
				int x = a[i].y - p.y;//x grab the diff between each block's y and No.1 block's y
				int y = a[i].x - p.x;//y grab the diff between each block's x and No.1 block's x
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check())
			{
				for (int i = 0; i < 4; i++)
				{
					a[i] = b[i];
				}
			}
		}

		//Tick
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;//Each delay time, automaticly move down 1 unit.
			}

			if (!check())
			{
				for (int i = 0; i < 4; i++)
				{
					field[b[i].y][b[i].x] = colorNum;
				}
				colorNum = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		dx = 0;
		rotate = 0;//Reset dx and rotate value to 0.
		delay = 0.3;//Reset delay to 0.3

		//Check Lines
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;
		}

		//Draw
		window.clear(Color::White);

		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				s.setPosition(j * 33, i * 33);
				window.draw(s);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			s.setPosition(a[i].x * 33, a[i].y * 33);
			window.draw(s);
		}

		
		window.display();
	}

	return 0;
}