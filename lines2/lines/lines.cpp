// lines.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
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


int score = 0;
int countBalls = 0;


void Draw(Sprite &fieldSprite, RenderWindow &window, Font &font, Text &text)
{
	
			window.clear(Color(255, 255, 255));
			window.draw(fieldSprite);


			ostringstream scoreString;    // объ€вили переменную
			scoreString << score;		//занесли в нее число очков, то есть формируем строку
			text.setString("Score: " + scoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 

			text.setPosition(360, 20);
			window.draw(text);

			for (int i = 0;i < 9;i++)
			{
				for (int k = 0;k < 9;k++)

					if (!ball[i][k].isFree)  window.draw(ball[i][k].ballSprite);
			}

			if (countBalls == 81)
			{
				Text GameOver("", font, 80);
				GameOver.setFillColor(Color::Black);
				GameOver.setString("Game Over");
				GameOver.setPosition(100, 360);
				window.draw(GameOver);
			}

			window.display();	

}

void Input(RenderWindow &window,Texture &ballTexture)
{
	Event event;

	while (window.pollEvent(event));
	{
		if (event.type == Event::Closed)
			window.close();

		int x, y;
		Vector2i pixelPos;
		Vector2f pos;
		bool isSelect = false;

		if ((event.type == Event::MouseButtonPressed) && (event.key.code == Mouse::Left))
		{
			 pixelPos = Mouse::getPosition(window);
			 pos = window.mapPixelToCoords(pixelPos);

			x = pos.x / 80;
			y = pos.y / 80 - 1;

			cout << x << y << endl;

			if ((!ball[x][y].isFree) && (ball[x][y].ballSprite.getGlobalBounds().contains(pos.x, pos.y)))
			{
				isSelect = true;
				cout << "isSelect";
			}
		}

         
		int  selectedX, selectedY;
		bool isMove = false;

		if ((event.type == Event::MouseButtonPressed) && (event.key.code == Mouse::Right) && (isSelect))
		{
			 pixelPos = Mouse::getPosition(window);
			 pos = window.mapPixelToCoords(pixelPos);

			selectedX = (int)(pos.x / 80);
			selectedY = (int)(pos.y / 80 - 1);

			cout << selectedX << selectedY;

			isSelect = false;
			isMove = true;
		}

		bool Throw = false;

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



}


void ThrowBalls(Texture &ballTexture)
{
	Color color[7] = { sf::Color::Red,sf::Color::Blue,sf::Color::Yellow, sf::Color::Cyan, sf::Color::White, sf::Color::Green, sf::Color::Magenta };
	int count = 0;

	srand(time(0));

	do
	{
		if (countBalls == 81) return;

		int x = (rand() % 9);
		int y = (rand() % 9);

		if (ball[x][y].isFree)
		{

			ball[x][y].ballSprite.setTexture(ballTexture);
			ball[x][y].color = color[rand() % 7];
			ball[x][y].ballSprite.setColor(ball[x][y].color);
			ball[x][y].ballSprite.setPosition(x * 80, y * 80 + 80);

			ball[x][y].isFree = false;
			count++;
			countBalls++;
		}
	} while (count != 5);

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
	font.loadFromFile("font.ttf");
	Text text("", font, 40);
	text.setFillColor(Color::Blue);



	ThrowBalls(ballTexture);

	while (countBalls != 81)
	{
		Draw(fieldSprite, window, font, text);
		Input(window, ballTexture);

		
	}



    return 0;
}

