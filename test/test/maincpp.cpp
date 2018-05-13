#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <vector>
#include <sstream>


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
bool DeletedLines = true;

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
			ball[x][y].color = color[rand() % 7];
			ball[x][y].ballSprite.setColor(ball[x][y].color);
			ball[x][y].ballSprite.setPosition(x*80, y*80+80);
			
			ball[x][y].isFree =false;
			count++;
			countBalls++;
		}	
	} while (count != 5);

}



void checkLines()
{
	DeletedLines = true;
	
	//Проверка линий по горизонтали

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
					for (int i = left;i <x;i++)ball[i][y].isFree = true;
					DeletedLines = false;
					score += count;
					countBalls -= count;
				}

				if (ball[x][y].isFree)
					count = 0;

				else
				{
					left = x;
					count = 1;
					color = ball[x][y].color;
				}
			}

		}

		if (count >= 3)
		{
			for (int i = left;i <= 8;i++)ball[i][y].isFree = true;
			DeletedLines = false;
			score += count;
			countBalls -= count;
		}

	}

	//Проверка линий по вертикали

	for (int x = 0;x < 9;x++)
	{
		Color color = Color::Black;

		int count = 0;
		int top = 0;


		for (int y = 0;y < 9;y++)
		{

			if ((color == ball[x][y].color) && (!ball[x][y].isFree))
				count++;


			if ((color != ball[x][y].color) && (!ball[x][y].isFree) || (ball[x][y].isFree))
			{
				if (count >= 5)
				{
					for (int i = top;i < y;i++)ball[x][i].isFree = true;
					DeletedLines = false;
					score += count;
					countBalls -= count;
				}

				if (ball[x][y].isFree)
					count = 0;


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
	bool Throw = false;
	
	
	ThrowBalls(ballTexture);
	
	while (window.isOpen())
	{
		Event event;

		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)

		

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
					isMove = true;
				}

				if (isMove)
				{
					if (ball[selectedX][selectedY].isFree)
					{
						ball[selectedX][selectedY].ballSprite.setTexture(ballTexture);
						ball[selectedX][selectedY].color = ball[x][y].color;
						ball[selectedX][selectedY].ballSprite.setColor(ball[x][y].color);
						ball[selectedX][selectedY].ballSprite.setPosition(selectedX * 80, selectedY * 80 + 80);

						ball[selectedX][selectedY].isFree = false;
						ball[x][y].isFree = true;
						isMove = false;
						Throw = true;
						
					}
				}
			}


	     checkLines();

			if ((Throw) &&(DeletedLines))
				
			{
				ThrowBalls(ballTexture);
				Throw = false;			
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

          if(countBalls==81)
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