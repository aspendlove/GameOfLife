#include <Arduino.h>
#include "Arduboy2.h"

Arduboy2Base arduboy;

uint8_t gameBoard[30][30]{};
uint8_t bufferOne[30];
uint8_t bufferTwo[30];
uint8_t cursorX;
uint8_t cursorY;
bool start = false;

void calculateNextGeneration(uint8_t[30], uint8_t[30][30]);
void displayGeneration();
uint8_t applyRule(uint8_t, uint8_t);
void writeBufferOne(uint8_t);
void writeBufferTwo(uint8_t);
void clearBoard();
void advance();

/*
1. take existing array and go down the first line
2. calculate each cell in the line based on it's neighbors
3. store those values in a buffer
4. move to the next line
5. store the values in a buffer, and write the previous buffer
6. move the previous buffer up to be used again

*/

void setup() {
    arduboy.begin();
    arduboy.setFrameRate(5);
    clearBoard();
    // gameBoard[0][0] = 0;
    // gameBoard[0][1] = 0;
    // gameBoard[1][0] = 1;
    // gameBoard[1][1] = applyRule(1, 1);

    // gameBoard[1][0] = 1;
    // gameBoard[0][1] = 1;
    // gameBoard[2][1] = 1;
    // gameBoard[1][2] = 1;

    // Serial.begin(9600);

    // gameBoard[3][3] = 1;
    // gameBoard[3][4] = 1;
    // gameBoard[3][5] = 1;

    // gameBoard[20][21] = 1;
    // gameBoard[21][22] = 1;
    // gameBoard[22][20] = 1;
    // gameBoard[22][21] = 1;
    // gameBoard[22][22] = 1;
}

void loop() {
    // put your main code here, to run repeatedly:
    if (!arduboy.nextFrame())
        return;
    arduboy.clear();

    arduboy.pollButtons();

    if (start) {
        advance();
        displayGeneration();

        if (arduboy.justPressed(B_BUTTON)) {
            start = false;
            clearBoard();
        }
    } else {
        if (arduboy.justPressed(LEFT_BUTTON)) {
            if (cursorX > 0) {
                cursorX--;
            }
        }
        if (arduboy.justPressed(RIGHT_BUTTON)) {
            if (cursorX <= 30) {
                cursorX++;
            }
        }
        if (arduboy.justPressed(UP_BUTTON)) {
            if (cursorY > 0) {
                cursorY--;
            }
        }
        if (arduboy.justPressed(DOWN_BUTTON)) {
            if (cursorY <= 30) {
                cursorY++;
            }
        }
        if (arduboy.justPressed(A_BUTTON)) {
            gameBoard[cursorX][cursorY] = !gameBoard[cursorX][cursorY];
        }
        if (arduboy.justPressed(B_BUTTON)) {
            start = true;
        }
        displayGeneration();
        arduboy.drawPixel((cursorX + 49), (cursorY + 17) + 1);
        arduboy.drawPixel((cursorX + 49) - 1, (cursorY + 17));
        arduboy.drawPixel((cursorX + 49) + 1, (cursorY + 17));
        arduboy.drawPixel((cursorX + 49), (cursorY + 17) - 1);
    }
    arduboy.display();
}

void advance() {
    for (uint8_t i = 0; i < 30; i += 2) {
        for (uint8_t j = 0; j < 30; j++) {
            bufferOne[j] = applyRule(i, j);
        }
        writeBufferTwo(i - 1);
        for (uint8_t j = 0; j < 30; j++) {
            bufferTwo[j] = applyRule(i + 1, j);
        }
        writeBufferOne(i);
    }
}

/*
  Any live cell with fewer than two live neighbours dies, as if by
  underpopulation.

  Any live cell with two or three live neighbours lives on to
  the next generation.

  Any live cell with more than three live neighbours dies,
  as if by overpopulation.

  Any dead cell with exactly three live neighbours
  becomes a live cell, as if by reproduction.
*/

uint8_t applyRule(uint8_t x, uint8_t y) {
    uint8_t neighborCount = 0;
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            if (i == 1 && j == 1) {
                continue;
            }
            neighborCount += gameBoard[(x - 1) + i][(y - 1) + j];
        }
    }
    if (gameBoard[x][y]) {
        if (neighborCount < 2 || neighborCount > 3) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (neighborCount == 3) {
            return 1;
        } else {
            return 0;
        }
    }
}

void writeBufferOne(uint8_t x) {
    for (uint8_t j = 0; j < 30; j++) {
        gameBoard[x][j] = bufferOne[j];
    }
}

void writeBufferTwo(uint8_t x) {
    if (x < 0) {
        return;
    }
    for (uint8_t j = 0; j < 30; j++) {
        gameBoard[x][j] = bufferTwo[j];
    }
}

void clearBoard() {
    for (uint8_t i = 0; i < 30; i++) {
        for (uint8_t j = 0; j < 30; j++) {
            gameBoard[i][j] = 0;
        }
    }
}

void displayGeneration() {
    arduboy.drawRect(48, 16, 32, 32);

    for (uint8_t i = 0; i < 30; i++) {
        for (uint8_t j = 0; j < 30; j++) {
            arduboy.drawPixel(i + 49, j + 17, gameBoard[i][j]);
        }
    }
};