#include <Arduino.h>
#include "Game.h"

#define PLAYER2 A0
#define PLAYER1 A1
#define SPEED A2

#define DIN 10
#define CS 9
#define CLK 11

Display display(DIN, CS, CLK, 2, 3);

Game game(PLAYER1, PLAYER2, SPEED, display);

void setup() {
	pinMode(PLAYER1, INPUT);
	pinMode(PLAYER2, INPUT);
	Serial.begin(9600);
}

void loop() {
	game.update();
	
	game.draw();
}