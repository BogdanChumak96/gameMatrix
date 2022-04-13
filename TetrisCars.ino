#include <GameBoy.h>
#include "Blocks.h"

GameBoy gb;
int rot = 0;
int x = 2, y = -1;
int acc = 1;
int speed = 200;
int score = 19;
int level = 0;


int pos = 2;

int modeCount = 0;
int modes = 0;

void setup() {
  randomSeed(analogRead(0) + analogRead(5));
  gb.begin(0);
  createBlock(random(0, 7));
  Serial.begin(9600);
}

void loop() {
  if (gb.getKey() == 2 and modeSelector() == 0) {
    gb.clearDisplay();
    modes = 1;
  }
  else  if (gb.getKey() == 2 and modeSelector() == 1) {
    gb.clearDisplay();
    modes = 2;
  }
  else if (gb.getKey() == 1)     {
    gb.clearDisplay();
    modes = 0;
  }
  switchMode(modes);
}


void switchMode(int mode) {
  switch (mode) {
    case 0: mainMenu();
      break;
    case 1: gameCar();
      break;
    case 2: gameTetris();
  }
}

int modeSelector() {
  if (gb.getKey() == 4) {
    modeCount++;
    delay(250);
    if (modeCount > 1) {
      modeCount = 0;
    }
  }
  else if (gb.getKey() == 5) {
    modeCount--;
    delay(250);
    if (modeCount < 0) {
      modeCount = 1;
    }
  }
  return modeCount;
}
void mainMenu() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      gb.wipePoint(i, j);
      gb.setLed(i, j, ARROWS[j][i]);
    }
  }
  
  if (modeSelector() == 0) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        gb.wipePoint(i, 8 + j);
        gb.setLed(i, 8 + j, CAR[j][i]);
      }
    }
  }
  if (modeSelector() == 1) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        gb.wipePoint(i, 8 + j);
        gb.setLed(i, 8 + j, TETRIS[j][i]);
      }
    }
  }
}



void gameTetris() {
  if (win() == true) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 16; j++) {
        gb.wipePoint(i, j);
        gb.setLed(i, j, WIN[j][i]);
      }
    }
    delay(2000);
    gb.clearDisplay();
    score = 0;
    level = 0;
  }

  if (loss() == true) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 16; j++) {
        gb.wipePoint(i, j);
      }
    }
    gb.sound(COLLISION);
    gb.testMatrix(10);
  }
  makeMove();
  if (gb.checkBlockCollision(gb.block[rot], x, y + 1)) {
    gb.memBlock(gb.block[rot], x, y);
    int lines = gb.fullLine();
    if (lines != 0) {
      score += lines;
      level += lines;
    }
    if (level >= 5) {
      gb.sound(SCORE);
      acc += 1;
      level = 0;
    }
    createBlock(random(0, 7));
  }
  else {
    y++;
  }
  gb.drawDisplay();
  drawBlock(gb.block[rot], x, y);
  delay(speed / acc);
}
















bool loss() {
  if (gb.checkBlockCollision(gb.block[rot], x, 0)) {
    return true;
  }
  else {
    return false;
  }
}
bool win() {
  if (score >= 20) {
    return true;
  }
  return false;
}
void drawBlock(byte arr[4][4], int x, int y) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (arr[j][i] == 1) {
        gb.drawPoint(x + i, y + j);
      }
    }
  }
}

void makeMove() {
  if (gb.getKey() == 4) {
    if (!gb.checkBlockCollision(gb.block[rot], x - 1, y)) {
      x--;
    }
  }
  if (gb.getKey() == 5) {
    if (!gb.checkBlockCollision(gb.block[rot], x + 1, y)) {
      x++;
    }
  }

  if (gb.getKey() == 3) {
    if (!gb.checkBlockCollision(gb.block[rot + 1], x + 1, y)) {
      if (rot == 3) {
        rot = 0;
      } else {
        rot++;
      }
    }
  }
  if (gb.getKey() == 6) {
    acc = 4;
  } else {
    acc = 1;
  }
}

void createBlock(int num) {
  x = 2; y = -1; rot = random(0, 4);
  if (num == 0) gb.generateBlock(gb.block, I_Block_1, I_Block_2, I_Block_3, I_Block_4);
  if (num == 1) gb.generateBlock(gb.block, Z_Block_1, Z_Block_2, Z_Block_3, Z_Block_4);
  if (num == 2) gb.generateBlock(gb.block, S_Block_1, S_Block_2, S_Block_3, S_Block_4);
  if (num == 3) gb.generateBlock(gb.block, L_Block_1, L_Block_2, L_Block_3, L_Block_4);
  if (num == 4) gb.generateBlock(gb.block, J_Block_1, J_Block_2, J_Block_3, J_Block_4);
  if (num == 5) gb.generateBlock(gb.block, T_Block_1, T_Block_2, T_Block_3, T_Block_4);
  if (num == 6) gb.generateBlock(gb.block, O_Block_1, O_Block_2, O_Block_3, O_Block_4);
}






//CARS ///

void gameCar() {

  int enemy_position = random(0, 10);

  if (enemy_position > 5) enemy_position = 2;
  else enemy_position = 5;

  for (int yT = -3; yT < 16; yT++) {
    creatMyCar(pos);
    Car(enemy_position, yT + 4);
    creatLine(yT);
    creatLine(yT + 5);
    creatLine(yT + 10);
    creatLine(yT + 15);
    creatLine(yT - 5);
    creatLine(yT - 10);
    creatLine(yT - 15);
    if (gb.getKey() == 5) {
      deletMyCar(pos);
      pos = 5;
    }
    if (gb.getKey() == 4) {
      deletMyCar(pos);
      pos = 2;
    }
    if (yT == 11) {
      gb.sound(SCORE);
    }
    if ((pos == enemy_position && yT == 8) or (gb.getKey() == 4 and (gb.checkCollision(2, 13))) or (gb.getKey() == 5 and (gb.checkCollision(5, 13)))) {
      gb.testMatrix(10);
      gb.clearDisplay();
      return;
    }

    delay(200);
    deletCar(enemy_position, yT + 4);
    deletLine(yT);
    deletLine(yT + 5);
    deletLine(yT + 10);
    deletLine(yT + 15);
    deletLine(yT - 5);
    deletLine(yT - 10);
    deletLine(yT - 15);
  }
}
void creatLine(int y) {
  gb.drawPoint(0, y);
  gb.drawPoint(0, y + 1);
  gb.drawPoint(0, y + 2);
  gb.drawPoint(7, y);
  gb.drawPoint(7, y + 1);
  gb.drawPoint(7, y + 2);
}
void deletLine(int y) {
  gb.wipePoint(0, y);
  gb.wipePoint(0, y + 1);
  gb.wipePoint(0, y + 2);
  gb.wipePoint(7, y);
  gb.wipePoint(7, y + 1);
  gb.wipePoint(7, y + 2);
}
void Car(int x, int y) {
  gb.drawPoint(x, y);
  gb.drawPoint(x, y - 1);
  gb.drawPoint(x - 1, y - 1);
  gb.drawPoint(x + 1, y - 1);
  gb.drawPoint(x, y - 2);
  gb.drawPoint(x - 1, y - 3);
  gb.drawPoint(x + 1, y - 3);
}
void deletCar(int x, int y) {
  gb.wipePoint(x, y);
  gb.wipePoint(x, y - 1);
  gb.wipePoint(x - 1, y - 1);
  gb.wipePoint(x + 1, y - 1);
  gb.wipePoint(x, y - 2);
  gb.wipePoint(x - 1, y - 3);
  gb.wipePoint(x + 1, y - 3);
}
void creatMyCar(int position) {
  gb.drawPoint(position, 12);
  gb.drawPoint(position, 13);
  gb.drawPoint(position - 1, 13);
  gb.drawPoint(position + 1, 13);
  gb.drawPoint(position, 14);
  gb.drawPoint(position - 1, 15);
  gb.drawPoint(position + 1, 15);
}

void  deletMyCar(int position) {
  gb.wipePoint(position, 12);
  gb.wipePoint(position, 13);
  gb.wipePoint(position - 1, 13);
  gb.wipePoint(position + 1, 13);
  gb.wipePoint(position, 14);
  gb.wipePoint(position - 1, 15);
  gb.wipePoint(position + 1, 15);
}
