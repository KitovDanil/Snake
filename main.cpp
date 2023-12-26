/*************************
* Author: Kitov D.A. *
* Date: 01.12.2023 *
* Name: Snake *
*************************/

#include <iostream>
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include "windows.h"

using namespace std;

// #################
// #               #
// #               #
// #               #
// #        1@@    #
// #          @    #
// #               #
// #               #
// #               #
// #               #
// #               #
// #################

struct field {
  vector<string> fieldForm;
  int height;
  int width;
};

struct snakePart {
  char symbol;
  int coordX;
  int coordY;
  snakePart* nextPart = NULL;
} head {'1', 5, 5};

enum Buttons { UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77 };

// Появление нового яблока на поле
void spawnAppleOnField(field &currentField) {
  int appleCoordX, appleCoordY;

  do {
    appleCoordX = 1 + rand() % (currentField.width - 2);
    appleCoordY = 1 + rand() % (currentField.height - 2);
  } while(currentField.fieldForm[appleCoordY][appleCoordX] != ' ');

  currentField.fieldForm[appleCoordY][appleCoordX] = '*';
}

// Рост змейки
void growSnake(snakePart* &tailPointer) {
  snakePart* newPart = new snakePart;
  newPart->symbol = '@';
  tailPointer->nextPart = newPart;
  tailPointer = newPart;
}

// Отрисовка змейки на поле
field drawSnakeOnField(field currentField, snakePart currentPart) {
  while(currentPart.nextPart != NULL) {
    currentField.fieldForm[currentPart.coordY][currentPart.coordX] = currentPart.symbol;
    currentPart = *currentPart.nextPart;
  }
  currentField.fieldForm[currentPart.coordY][currentPart.coordX] = currentPart.symbol;
  return currentField;
}

// Проверка столкновения змейки с различными объектами: 0 - нет столкновения, 1 - стена, 2 - яблоко
int checkCollision(field currentField, snakePart currentPart) {
  int collisionType = 0;
  snakePart currentHead = currentPart;
  while(currentPart.nextPart != NULL) {
    currentPart = *currentPart.nextPart;
    if(currentHead.coordX == currentPart.coordX && currentHead.coordY == currentPart.coordY) {
      collisionType = 1;
    }
  }
  if(currentField.fieldForm[currentHead.coordY][currentHead.coordX] == '#') {
    collisionType = 1;
  } else if(currentField.fieldForm[currentHead.coordY][currentHead.coordX] == '*') {
    collisionType = 2;
  }

  return collisionType;
}

// Передвижение остального тела, когда голова уже смещена
void moveBody(snakePart *currentPart, int oldCoordX, int oldCoordY) {
  int tempOldCoordX, tempOldCoordY;

  while(currentPart->nextPart != NULL) {
    currentPart = currentPart->nextPart;

    tempOldCoordX = oldCoordX;
    oldCoordX = currentPart->coordX;
    currentPart->coordX = tempOldCoordX;

    tempOldCoordY = oldCoordY;
    oldCoordY = currentPart->coordY;
    currentPart->coordY = tempOldCoordY;
  }
}

void refreshField(field &currentField, snakePart &snake) {
  int snakeDirection = LEFT;
  bool isGameContinued = true;
  int score = 0;
  snakePart* tailPointer = &snake;

  do {
    spawnAppleOnField(currentField);
  } while(currentField.fieldForm[snake.coordY][snake.coordX] == '*');

  while(isGameContinued) {
    Sleep(500);
    system("cls");

    while(_kbhit()) {
      snakeDirection = _getch();
    }

    int oldCoordX = snake.coordX, oldCoordY = snake.coordY;
    switch(snakeDirection) {
      case UP:
        snake.coordY -= 1;
        if(snake.coordY < 0) {
          snake.coordY = currentField.height - 1;
        }
        break;
      case DOWN:
        snake.coordY += 1;
        snake.coordY %= currentField.height;
        break;
      case LEFT:
        snake.coordX -= 1;
        if(snake.coordX < 0) snake.coordX = currentField.width - 1;
        break;
      case RIGHT:
        snake.coordX += 1;
        snake.coordX %= currentField.width;
        break;
      default:
        break;
    }

    if(checkCollision(currentField, snake) == 1) {
      isGameContinued = false;
    }
    if(checkCollision(currentField, snake) == 2) {
      growSnake(tailPointer);
      currentField.fieldForm[snake.coordY][snake.coordX] = ' ';
      spawnAppleOnField(currentField);
      ++score;
    }

    moveBody(&snake, oldCoordX, oldCoordY);

    field fieldToPrint = drawSnakeOnField(currentField, snake);

    for(int fieldRowIndex = 0; fieldRowIndex < fieldToPrint.fieldForm.size(); ++fieldRowIndex) {
      cout << fieldToPrint.fieldForm[fieldRowIndex] << endl;
    }
    cout << "Счет: " << score << endl;
  }
}


int main()
{
  srand(time(NULL));
  setlocale(LC_ALL, "RUS");

  string fieldForm[] = {
        "#################",
        "#               #",
        "#               #",
        "#               #",
        "#               #",
        "#               #",
        "#               #",
        "#               #",
        "#               #",
        "#               #",
        "#               #",
        "#################"
  };
  field mainField;
  snakePart snake = head;

  mainField.height = 12;
  mainField.width = 17;
  for(int fieldRowIndex = 0; fieldRowIndex < mainField.height; ++fieldRowIndex) {
    mainField.fieldForm.push_back(fieldForm[fieldRowIndex]);
  }

  refreshField(mainField, snake);
  return 0;
}
