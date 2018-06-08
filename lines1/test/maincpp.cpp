#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <vector>
#include <sstream>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

using namespace sf;
using namespace std;


struct Ball
{
	bool isFree = true;
	Color color;
	Sprite ballSprite;
	
} ball[9][9];


int score=0;
int countBalls=0;

void ThrowBalls(Texture &ballTexture)
{
	Color color[7] = { sf::Color::Red,sf::Color::Blue,sf::Color::Yellow, sf::Color::Cyan, sf::Color::White, sf::Color::Green, sf::Color::Magenta };
	int count = 0;
	
	srand(time(0)); 
	
	do
	{
		if (countBalls == 81) return;
		int x = (rand() % 9);
		int y = (rand()% 9);

		if (ball[x][y].isFree)
		{		
			ball[x][y].ballSprite.setTexture(ballTexture);
			ball[x][y].color = color[rand() %7];
			ball[x][y].ballSprite.setColor(ball[x][y].color);
			ball[x][y].ballSprite.setPosition(x*80, y*80+80);
			
			ball[x][y].isFree =false;
			count++;
			countBalls++;
		}	
	} while (count != 3);

}


bool pathSearch(int startX, int startY, int endX, int endY)
{
	
	int map[9][9];
	for (int x = 0;x < 9;x++)
	{
		for (int y = 0;y < 9;y++)
		{
			if (ball[x][y].isFree) map[x][y] = -2;
			else map[x][y] = -1;
		}
}
	map[startX][startY] = 0;
	map[endX][endY] = -2;
   
	int dx[4] = { 1, 0, -1, 0 };  
	int dy[4] = { 0, 1, 0, -1 };   
	
	int step = 0;
    int  x, y, k;
    bool stop;
	bool added;

	do {
		stop = true;
		
		for (y = 0; y < 9; ++y)
			for (x = 0; x < 9; ++x)
				if (map[x][y] == step)                         
				{
					for (k = 0; k < 4; ++k)                   
					{
						int iy = y + dy[k],
							ix = x + dx[k];
						if (iy >= 0 && iy < 9 && ix >= 0 && ix < 9 &&
							map[ix][iy] == -2)
						{
							stop = false;
							map[ix][iy] = step + 1;     

						}
					}		
				}
		
		step++;

	} while (!stop&& map[endX][endY] == -2);

	if (map[endX][endY]==-2) return false;
	else return true;
}




bool checkLines1()	//Проверка линий по горизонтали

{
	bool DeletedLines = true;

	for (int y = 0;y < 9;y++)
	{
		Color color = Color::Black;
		int count = 0;
		int left = 0;

		for (int x = 0;x < 9;x++)
		{

			if ((color == ball[x][y].color) && (!ball[x][y].isFree))
				count++;


			if ((color != ball[x][y].color) && (!ball[x][y].isFree) || (ball[x][y].isFree))
			{
				if (count >= 5)
				{
					for (int i = left;i < x;i++)ball[i][y].isFree = true;
					DeletedLines = false;
					score += count;
					countBalls -= count;
				}

				if (ball[x][y].isFree)
				{
					count = 0;
					color = Color::Black;
				}

				else
				{
					left = x;
					count = 1;
					color = ball[x][y].color;
				}
			}
		}

		if (count >= 5)
		{
			for (int i = left;i <= 8;i++)ball[i][y].isFree = true;
			DeletedLines = false;
			score += count;
			countBalls -= count;
		}

	}

	return DeletedLines;
}



bool checkLines2()//Проверка линий по вертикали

{
	bool DeletedLines = true;
	
	for (int x = 0;x < 9;x++)
	{
		Color color = Color::Black;
		int count = 0;
		int top = 0;

		for (int y = 0;y < 9;y++)
		{
			if ((color == ball[x][y].color) && (!ball[x][y].isFree))
				count++;

			if ((color != ball[x][y].color) && !ball[x][y].isFree || (ball[x][y].isFree))
			{
				if (count >= 5)
				{
					for (int i = top;i < y;i++)ball[x][i].isFree = true;
					DeletedLines = false;
					score += count;
					countBalls -= count;
				}

				if (ball[x][y].isFree)
				{
					count = 0;
					color = Color::Black;
				}

				else
				{
					top = y;
					count = 1;
					color = ball[x][y].color;
				}
			}
		}
		if (count >= 5)
		{
			for (int i = top;i <= 8;i++) ball[x][i].isFree = true;
			DeletedLines = false;
			score += count;
			countBalls -= count;
		}
	}

	return DeletedLines;

}


bool checkLines3()//Проверка линий по диагонали слева направо над главной диагональю
{
	bool DeletedLines = true;

	for (int x = 0;x <= 5;x++)
	{
		int i = x;
		int k = 0;
		int count = 0;
		Color color = Color::Black;
		int topI = x;
		int topK = 0;

		do {
			if (!ball[i][k].isFree && (ball[i][k].color == color))
				count++;

			if ((!ball[i][k].isFree && (ball[i][k].color != color)) || (ball[i][k].isFree))
			{
				if (count >= 5)
				{
					for (;((topI < i) && (topK < k));((topI++) && (topK++))) ball[topI][topK].isFree = true;
					DeletedLines = false;
					score += count;
					countBalls -= count;
				}
				if (ball[i][k].isFree)
				{
					count = 0;
					color = Color::Black;
				}

				else
				{
					color = ball[i][k].color;
					count = 1;
					topI = i;
					topK = k;
				}
			}
			i++;
			k++;
		} while ((i < 9) && (k < 9));

		if (count >= 5)
		{
			for (;((topI < 9) && (topK < 9));((topI++) && (topK++))) ball[topI][topK].isFree = true;
			DeletedLines = false;
			score += count;
			countBalls -= count;
		}
	}

	return DeletedLines;
}


bool checkLines4()
{
	bool DeletedLines = true;

    for (int y = 0;y <= 5;y++)
	{
		int i = 0;
		int k = y;
		int count = 0;
		Color color = Color::Black;
		int topI = 0;
		int topK = y;

		do {
			if (!ball[i][k].isFree && (ball[i][k].color == color))
				count++;

			if ((!ball[i][k].isFree && (ball[i][k].color != color)) || (ball[i][k].isFree))
			{
				if (count >= 5)
				{
					do {
					    ball[topI][topK].isFree = true;
						DeletedLines = false;
						
						topI++;
						topK++;
					} while ((topI < i) && (topK < k));
					
					score += count;
					countBalls -= count;
				}
				if (ball[i][k].isFree)
				{
					count = 0;
					color = Color::Black;
				}

				else
				{
					color = ball[i][k].color;
					count = 1;
					topI = i;
					topK = k;
				}
			}
			i++;
			k++;
		} while ((i < 9) && (k < 9));

		if (count >= 5)
		{
			do {
				ball[topI][topK].isFree = true;
				DeletedLines = false;
				topI++;
				topK++;
			} while ((topI < i) && (topK < k));
			score += count;
			countBalls -= count;
		}
	}
	return DeletedLines;
}


bool checkLines5()//проверка диагоналей справо налево
{
	bool DeletedLines = true;

	for (int x = 4;x <9;x++)
	{
		int i = x;
		int k = 0;
		int count = 0;
		Color color = Color::Black;
		int topI = x;
		int topK = 0;

		do {
			if ((!ball[i][k].isFree) && (ball[i][k].color == color))
				count++;

			if ((!ball[i][k].isFree && (ball[i][k].color != color)) || (ball[i][k].isFree))
			{
				if (count >= 5)
				{
					do {
						ball[topI][topK].isFree = true;
						topI--;
						topK++;
					} while (topI >= i && topK < k);
					DeletedLines = false;
					score += count;
					countBalls -= count;
				}
				if (ball[i][k].isFree)
				{
					count = 0;
					color = Color::Black;
				}

				else
				{
					color = ball[i][k].color;
					count = 1;
					topI = i;
					topK = k;
				}
			}
			i--;
			k++;
		} while ((i >=0) && (k < 9));

		if (count >= 5)
		{
			do {
				 ball[topI][topK].isFree = true; 
				topI--;
				topK++;
			} while (topI >= 0 && topK < 9);
			DeletedLines = false;
			score += count;
			countBalls -= count;
		}
	}

	return DeletedLines;
}

bool checkLines6() 
{
	bool DeletedLines = true;

	for (int y = 1;y <5;y++)
	{
		int i = 8;
		int k = y;
		int count = 0;
		Color color = Color::Black;
		int topI = 8;
		int topK = y;

		do {
			if ((!ball[i][k].isFree) && (ball[i][k].color == color))
				count++;

			if ((!ball[i][k].isFree && (ball[i][k].color != color)) || (ball[i][k].isFree))
			{
				if (count >= 5)
				{
					do {
						ball[topI][topK].isFree = true;
						topI--;
						topK++;
						count--;
					} while (count>0);
					DeletedLines = false;
					score += count;
					countBalls -= count;
				}
				if (ball[i][k].isFree)
				{
					count = 0;
					color = Color::Black;
				}

				else
				{
					color = ball[i][k].color;
					count = 1;
					topI = i;
					topK = k;
				}
			}
			i--;
			k++;
		} while ((i >= 0) && (k < 9));

		if (count >= 5)
		{
			do {
				ball[topI][topK].isFree = true;
				topI--;
				topK++;
				count--;
			} while (count>0);
			DeletedLines = false;
			score += count;
			countBalls -= count;
		}
	}
	return DeletedLines;

}


bool checkLines()
{
	if (checkLines1() && checkLines2() && checkLines3() && checkLines4()&&checkLines5()&&checkLines6()) return true;
	else return false;
}

int main()
{
	setlocale(0, "rus");
	
	RenderWindow window(VideoMode(720, 800), "lines");	
	
	Texture fieldTexture;
	fieldTexture.loadFromFile("field2.png");
	
	Sprite fieldSprite;
	fieldSprite.setTexture(fieldTexture);
	
	Image ballImage;
	ballImage.loadFromFile("ball.png");

	Texture ballTexture;
	ballTexture.loadFromImage(ballImage);
		
	Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setFillColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	

	int x, y;

	bool isMove = false;
	bool isSelect = false;
	bool Throw =true;
	

	int FPS = 80; 
	int msSleepTime = 1000 / FPS;
	sf::Clock sleepClock;

	
	while (window.isOpen())
	{
	
		sf::Time sleepTime = sf::milliseconds(msSleepTime - sleepClock.getElapsedTime().asMilliseconds());
		if (sleepTime.asMilliseconds() > 0) sf::sleep(sleepTime);
		
		sleepClock.restart();

		Event event;

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		
		if (Throw && checkLines())
				ThrowBalls(ballTexture);
	
		Throw = false;
	
		while (window.pollEvent(event));
		{

			if (event.type == Event::Closed)
				window.close();

	
				if ((event.type == Event::MouseButtonPressed) && (event.key.code == Mouse::Left))
				{
					x = pos.x / 80;
					y = pos.y / 80 - 1;


					if ((!ball[x][y].isFree) && (ball[x][y].ballSprite.getGlobalBounds().contains(pos.x, pos.y)))
						isSelect = true;
				}
				
				int  selectedX, selectedY;

				if ((event.type == Event::MouseButtonPressed) && (event.key.code == Mouse::Right) && (isSelect))
				{
					selectedX = (int)(pos.x / 80);
					selectedY = (int)(pos.y / 80 - 1);

					isSelect = false;
					if (ball[selectedX][selectedY].isFree)isMove = true;
				}

				if(isMove)
				{
					if (pathSearch(x, y, selectedX, selectedY)==true)
					{
						ball[selectedX][selectedY].ballSprite.setTexture(ballTexture);
						ball[selectedX][selectedY].color = ball[x][y].color;
						ball[selectedX][selectedY].ballSprite.setColor(ball[x][y].color);
						ball[selectedX][selectedY].ballSprite.setPosition(selectedX * 80, selectedY * 80 + 80);

						ball[selectedX][selectedY].isFree = false;
						ball[x][y].isFree = true;
						Throw = true;
					}
				 	    isMove = false;
				}
			}
				
			
		{
			window.clear(Color(255, 255, 255));
			window.draw(fieldSprite);

		    ostringstream scoreString;    // объявили переменную
			scoreString << score;		//занесли в нее число очков, то есть формируем строку
			text.setString("Score: " + scoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 

			text.setPosition(360, 20);
			window.draw(text);

			for (int i = 0;i < 9;i++)
			{
				for (int k = 0;k < 9;k++)
					if (!ball[i][k].isFree)  window.draw(ball[i][k].ballSprite);
			}


			if (countBalls >= 81)
	{
		Text GameOver("", font, 80);
		GameOver.setFillColor(Color::Black);
		GameOver.setString("Game Over");
		GameOver.setPosition(100, 360);
		window.draw(GameOver);
	}
    
			window.display();		
		}
	}

	
	return 0;
}