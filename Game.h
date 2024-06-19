#pragma once
#include "KipoLed.h"

struct Point
{
	byte x, y;
};

void changeBitAt(byte x, byte y, bool state, unsigned* data)
{
	unsigned mask = 1 << (15 - x);
	data[y] |= (data[y] & ~mask) | mask; 
}

bool getBitAt(byte x, byte y, unsigned* data)
{
	unsigned mask = 1 << (15 - x);
	return (data[y] & mask); 
}

void clear(unsigned* data, byte size)
{
	while (size)
	{
		data[--size] = 0;
	}
}


class Game
{
public:
	Game(int player1Pin, int player2Pin, int speedPin, Display& display)
		: display(display), player1Pin(player1Pin), player2Pin(player2Pin), speedPin(speedPin), ball({{7, 4}, 1, 1})
	{
	}

	void update()
	{
		player1 = analogRead(player1Pin) >> 7; // from 0-1023 to 0-7
		player2 = analogRead(player2Pin) >> 7; 
		unsigned speed = analogRead(speedPin) >> 2;

		if (player1 > 5) player1 = 5;
		if (player2 > 5) player2 = 5;

		timeToUpdate = speed + 25;
		if (millis() - timeSinceLastUpdate < timeToUpdate)
			return;
		
		timeSinceLastUpdate = millis();

		if (ball.coords.y == 0 || ball.coords.y == 7)
			ball.yDir = -ball.yDir;

		if (ball.coords.x == 0)
		{
			display.setAll(p2W);
			delay(2500);
			ball.coords = {7, 4};
			ball.xDir = -ball.xDir;
		}
		else if (ball.coords.x == 15)
		{
			display.setAll(p1W);
			delay(2500);
			ball.coords = {7, 4};
			ball.xDir = -ball.xDir;
		}

		else if (ball.coords.x == 1)
		{
			if (abs(ball.coords.y + ball.yDir - (player1 + 1)) < 2)
				ball.xDir = -ball.xDir;
		}

		else if (ball.coords.x == 14)
		{
			if (abs(ball.coords.y + ball.yDir - (player2 + 1)) < 2)
				ball.xDir = -ball.xDir;
		}

		ball.coords.x += ball.xDir;
		ball.coords.y += ball.yDir;
	}

	void draw()
	{
		clear(grid, 8);

		changeBitAt(0, player1, true, grid);
		changeBitAt(0, player1 + 1, true, grid);
		changeBitAt(0, player1 + 2, true, grid);
		changeBitAt(15, player2, true, grid);
		changeBitAt(15, player2 + 1, true, grid);
		changeBitAt(15, player2 + 2, true, grid);

		changeBitAt(ball.coords.x, ball.coords.y, true, grid);

		display.setAll(grid);

		delay(1);
	}

private:
    Display& display;

	unsigned grid[8];

	unsigned long timeSinceLastUpdate, timeToUpdate;

	int8_t player1, player2;

    const int8_t player1Pin, player2Pin;
    const int8_t speedPin;

	struct Ball
	{
		Point coords;
		int8_t xDir, yDir;
	} ball;

    unsigned p1W[8] = {
        0b1111111100000000,
        0b1011110100100000,
        0b1011110100100000,
        0b1011110100100000,
        0b1010010100100000,
        0b1010010100100000,
        0b1000000100111100,
        0b1111111100000000,
    };

    unsigned p2W[8] = {
        0b0000000011111111,
        0b0010000010111101,
        0b0010000010111101,
        0b0010000010111101,
        0b0010000010100101,
        0b0010000010100101,
        0b0011110010000001,
        0b0000000011111111,
    };

};