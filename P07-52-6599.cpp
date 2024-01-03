

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glut.h>
#include <cmath>
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>

// Line properties
int numLines = 25;
float lineSpeed = 0.5f;
float lineLength = 100.0f;
float lineThickness = 2.0f;

// Structure to represent a line
struct Line {
	float x;
	float y;
	float angle;
	float alpha;
	float alphaChange;
};

Line lines[25];

bool Lost = false;
bool Won = false;

int ScoreUp = 25;
int Score = 0;
int Time = 100;
int Health = 330;
int blueD = 1;

int Speed = 10;
int TransX = 0;
int TransY = 0;
int LocX = 450;
int LocY = 300;
int RotZ = 0;

int targetDir = 1;
float targetScale = 1;
int targetx = 50;
int targety = 50;

int obs1x = 0;
int obs1y = 0;
int obs2x = 0;
int obs2y = 0;
int obs3x = 0;
int obs3y = 0;
int obs4x = 0;
int obs4y = 0;



bool power1 = false;
bool power2 = false;
bool power1Active = false;
bool power2Active = false;

float powerUpRot = -45;
int powerUpDir = 1;
int power1x = 0;
int power1y = 0;
int power2x = 0;
int power2y = 0;
int power1Time = 5;
int power2Time = 5;

bool collect1 = false;
bool collect2 = false;
bool collect3 = false;
bool collect4 = false;

float collectRot = 0;
int collect1x = 0;
int collect1y = 0;
int collect2x = 0;
int collect2y = 0;
int collect3x = 0;
int collect3y = 0;
int collect4x = 0;
int collect4y = 0;

int collect[4][2];
int powerup[2][2];
int obstacle[4][2];

bool flag = false;

const float PI = 3.14159265358979323846;

void Anim(void);

float getRandomFloat(float min, float max) {
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void initializeLines() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < numLines; ++i) {
		lines[i].x = getRandomFloat(0, 900);
		lines[i].y = getRandomFloat(0, 600);
		lines[i].angle = getRandomFloat(0, 360);
		lines[i].alpha = 1.0f;
		lines[i].alphaChange = getRandomFloat(0.01f, 0.03f);
	}
}

void updateLines() {
	for (int i = 0; i < numLines; ++i) {
		lines[i].x += lineSpeed * std::cos(lines[i].angle * 3.14159f / 180.0f);
		lines[i].y += lineSpeed * std::sin(lines[i].angle * 3.14159f / 180.0f);
		lines[i].alpha -= lines[i].alphaChange;

		if (lines[i].x < 0 || lines[i].x > 900 || lines[i].y < 0 || lines[i].y > 600) {
			lines[i].x = getRandomFloat(0, 900);
			lines[i].y = getRandomFloat(0, 600);
			lines[i].angle = getRandomFloat(0, 360);
			lines[i].alpha = 1.0f;
			lines[i].alphaChange = getRandomFloat(0.01f, 0.03f);
		}
	}
	glutPostRedisplay();
}

/// ///////////////////////////////////////////////////////////End Background
void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	Time--;
	if (Time == 00 && !Won) {
		Lost = true;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, time, 0);

}

void power1time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	if (power1Active) {
		power1Time--;
		if (power1Time <= 0) {
			power1Active = false;
			Speed = 10;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(1000, power1time, 0);
}

void power2time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	if (power2Active) {
		power2Time--;
		if (power2Time <= 0) {
			power2Active = false;
			ScoreUp = 25;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(1000, power2time, 0);
}

int* GenerateRandomxy() {
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> distancex(30, 870);
	std::uniform_int_distribution<int> distancey(30, 470);
	int numx = distancex(engine);
	int numy = distancey(engine);

	for (int i = 0;i < 4;i++) {
		if (numx <= collect[i][0] + 35 && numx >= collect[i][0] - 35 && numy <= collect[i][1] + 35 && numy >= collect[i][1] - 35) {
			numx = distancex(engine);
			numy = distancey(engine);
			i--;
		}

	}
	for (int i = 0;i < 2;i++) {
		if (numx <= powerup[i][0] + 35 && numx >= powerup[i][0] - 35 && numy <= powerup[i][1] + 35 && numy >= powerup[i][1] - 35) {
			numx = distancex(engine);
			numy = distancey(engine);
			i--;
		}
	}
	for (int i = 0;i < 4;i++) {
		if (numx <= obstacle[i][0] + 45 && numx >= obstacle[i][0] - 45 && numy <= obstacle[i][1] + 45 && numy >= obstacle[i][1] - 45) {
			numx = distancex(engine);
			numy = distancey(engine);
			i--;
		}
	}
	for (int i = 0;i < 1;i++) {
		if (numx <= targetx + 45 && numx >= targetx - 45 && numy <= targety + 45 && numy >= targety - 45) {
			numx = distancex(engine);
			numy = distancey(engine);
			i--;
		}
	}
	for (int i = 0;i < 1;i++) {
		if (numx <= 480 && numx >= 420 && numy <= 330 && numy >= 270) {
			numx = distancex(engine);
			numy = distancey(engine);
			i--;
		}
	}

	int num[2] = { numx,numy };
	return num;
}

void Loc(int k, int x, int y)
{
	int TransXOld = TransX;
	int TransYOld = TransY;
	int LocXOld = LocX;
	int LocYOld = LocY;

	if (k == GLUT_KEY_RIGHT) {
		TransX += Speed;
		LocX += Speed;
		RotZ = -90;
	}
	if (k == GLUT_KEY_LEFT)
	{
		TransX -= Speed;
		LocX -= Speed;
		RotZ = 90;
	}
	if (k == GLUT_KEY_UP)
	{
		TransY += Speed;
		LocY += Speed;
		RotZ = 0;
	}
	if (k == GLUT_KEY_DOWN)
	{
		TransY -= Speed;
		LocY -= Speed;
		RotZ = 180;
	}

	if ((LocX <= obs1x + 45 && LocX >= obs1x - 45) && (LocY <= obs1y + 45 && LocY >= obs1y - 45)) {

		TransX = TransXOld;
		TransY = TransYOld;
		LocX = LocXOld;
		LocY = LocYOld;

		Health -= 40;
	}
	if ((LocX <= obs2x + 45 && LocX >= obs2x - 45) && (LocY <= obs2y + 45 && LocY >= obs2y - 45)) {
		TransX = TransXOld;
		TransY = TransYOld;
		LocX = LocXOld;
		LocY = LocYOld;
		Health -= 40;
	}
	if ((LocX <= obs3x + 45 && LocX >= obs3x - 45) && (LocY <= obs3y + 45 && LocY >= obs3y - 45)) {
		TransX = TransXOld;
		TransY = TransYOld;
		LocX = LocXOld;
		LocY = LocYOld;
		Health -= 40;
	}
	if ((LocX <= obs4x + 45 && LocX >= obs4x - 45) && (LocY <= obs4y + 45 && LocY >= obs4y - 45)) {
		TransX = TransXOld;
		TransY = TransYOld;
		LocX = LocXOld;
		LocY = LocYOld;
		Health -= 40;
	}
	////////////////////////////////Barrier
	if (LocX >= 880 || LocX <= 20) {
		if (LocX >= 880) {
			TransX -= Speed;
			LocX -= Speed;
		}
		if (LocX <= 20) {
			TransX += Speed;
			LocX += Speed;
		}
		Health -= 40;
	}
	if ((LocY >= 480)) {
		TransY -= Speed;
		LocY -= Speed;
		Health -= 40;
	}
	if ((LocY <= 20)) {
		TransY += Speed;
		LocY += Speed;
		Health -= 40;
	}

	/////////////////////Collectable
	if ((LocX <= collect1x + 25 && LocX >= collect1x - 25) && (LocY <= collect1y + 25 && LocY >= collect1y - 25)) {
		Score += ScoreUp;
		collect1x = 0;
		collect1y = 0;
		collect1 = true;
	}
	if ((LocX <= collect2x + 25 && LocX >= collect2x - 25) && (LocY <= collect2y + 25 && LocY >= collect2y - 25)) {
		Score += ScoreUp;
		collect2x = 0;
		collect2y = 0;
		collect2 = true;
	}
	if ((LocX <= collect3x + 25 && LocX >= collect3x - 25) && (LocY <= collect3y + 25 && LocY >= collect3y - 25)) {
		Score += ScoreUp;
		collect3x = 0;
		collect3y = 0;
		collect3 = true;
	}
	if ((LocX <= collect4x + 25 && LocX >= collect4x - 25) && (LocY <= collect4y + 25 && LocY >= collect4y - 25)) {
		Score += ScoreUp;
		collect4x = 0;
		collect4y = 0;
		collect4 = true;
	}
	////////////////////PowerUp
	if ((LocX <= power1x + 25 && LocX >= power1x - 25) && (LocY <= power1y + 25 && LocY >= power1y - 25)) {
		power1x = 0;
		power1y = 0;
		power1 = true;
		power1Active = true;
		Speed = 20;
	}
	if ((LocX <= power2x + 25 && LocX >= power2x - 25) && (LocY <= power2y + 25 && LocY >= power2y - 25)) {
		power2x = 0;
		power2y = 0;
		power2 = true;
		power2Active = true;
		ScoreUp = 50;

	}
	if ((LocX <= targetx + 25 && LocX >= targetx - 25) && (LocY <= targety + 25 && LocY >= targety - 25)) {
		Won = true;
	}
	glutPostRedisplay();//redisplay to update the screen with the changes
}


/// //////////////////////////////////////////////////Display

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	for (int i = 0; i < numLines; ++i) {
		glColor4f(getRandomFloat(0, 1), getRandomFloat(0, 1), getRandomFloat(0, 1), lines[i].alpha);
		glLineWidth(lineThickness);
		glBegin(GL_LINES);
		glVertex2f(lines[i].x, lines[i].y);
		glVertex2f(lines[i].x + lineLength * std::cos(lines[i].angle * 3.14159f / 180.0f),
			lines[i].y + lineLength * std::sin(lines[i].angle * 3.14159f / 180.0f));
		glEnd();
	}
	glPopMatrix();

	char* p0s[25];
	if (!Won && !Lost && Health > 130) {
		if (!flag) {

			for (int i = 0;i < 4;i++) {
				int* temp = GenerateRandomxy();
				obstacle[i][0] = temp[0];
				obstacle[i][1] = temp[1];
			}

			for (int i = 0;i < 4;i++) {
				int* temp = GenerateRandomxy();
				collect[i][0] = temp[0];
				collect[i][1] = temp[1];
			}
			for (int i = 0;i < 2;i++) {
				int* temp = GenerateRandomxy();
				powerup[i][0] = temp[0];
				powerup[i][1] = temp[1];
			}

			obs1x = obstacle[0][0];
			obs1y = obstacle[0][1];
			obs2x = obstacle[1][0];
			obs2y = obstacle[1][1];
			obs3x = obstacle[2][0];
			obs3y = obstacle[2][1];
			obs4x = obstacle[3][0];
			obs4y = obstacle[3][1];

			collect1x = collect[0][0];
			collect1y = collect[0][1];
			collect2x = collect[1][0];
			collect2y = collect[1][1];
			collect3x = collect[2][0];
			collect3y = collect[2][1];
			collect4x = collect[3][0];
			collect4y = collect[3][1];

			power1x = powerup[0][0];
			power1y = powerup[0][1];
			power2x = powerup[1][0];
			power2y = powerup[1][1];

			flag = true;
		}

		sprintf((char*)p0s, "Score: %d", Score);
		glPushMatrix();
		glColor3f(1, 0, 0);
		print(450, 550, (char*)p0s);
		glPopMatrix();

		sprintf((char*)p0s, "Time: %d", Time);
		glPushMatrix();
		glColor3f(1, 1, 1);
		print(750, 550, (char*)p0s);
		glPopMatrix();

		sprintf((char*)p0s, "Heath:");
		glPushMatrix();
		glColor3f(0, 1, 0);
		print(50, 550, (char*)p0s);
		glLineWidth(5);
		glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(130, 555, 0.0f);
		glVertex3f(Health, 555, 0.0f);
		glEnd();
		glPopMatrix();
		////////////////////////////////////////Power up String
		if (power1Active) {
			sprintf((char*)p0s, "x2 Speed: %d", power1Time);
			glPushMatrix();
			glColor3f(100, 84.3, 0);
			print(600, 560, (char*)p0s);
			glPopMatrix();
		}
		if (power2Active) {
			sprintf((char*)p0s, "x2 Score: %d", power2Time);
			glPushMatrix();
			glColor3f(100, 84.3, 0);
			print(600, 530, (char*)p0s);
			glPopMatrix();
		}
		//////////////////////////////////////////Target Start

		glPushMatrix();
		glTranslated(targetx, targety, 0);
		glScaled(targetScale, targetScale, 1);
		glTranslated(-targetx, -targety, 0);

		float colors[5][3] = {
			{1.0, 0.0, 0.0},    // Red
			{1.0, 1.0, 1.0},    // White
			{1.0, 0.0, 0.0},    // Red
			{1.0, 1.0, 1.0},    // White
			{0.0, 0.0, 0.0}     // Black
		};

		// Calculate the radius step for each ring
		float radiusStep = 30 / 5;

		// Loop through each ring
		for (int i = 0; i < 5; i++) {
			glBegin(GL_TRIANGLE_FAN);
			glColor3f(colors[i % 5][0], colors[i % 5][1], colors[i % 5][2]);
			glVertex2f(targetx, targety); // Center of the circle
			for (int j = 0; j <= 360; j += 10) {
				float angle = j * PI / 180;
				float x = targetx + (30 - i * radiusStep) * cos(angle);
				float y = targety + (30 - i * radiusStep) * sin(angle);
				glVertex2f(x, y);
			}
			glEnd();
		}
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.8, 0.8, 0.8);

		////////////////////////////Begin Barrier

		// Draw the horizontal lines
		glBegin(GL_LINES);
		for (float y = 500.0; y <= 505.0; y += 5.0) {
			glVertex2f(0.0, y);
			glVertex2f(900.0, y);
		}
		for (float y = 0; y <= 5.0; y += 5.0) {
			glVertex2f(0.0, y);
			glVertex2f(900.0, y);
		}
		for (float y = 0; y <= 505; y += 100) {
			glVertex2f(0.0, y);
			glVertex2f(10.0, y);
		}
		for (float y = 0; y <= 505; y += 100) {
			glVertex2f(900.0, y);
			glVertex2f(890.0, y);
		}

		glEnd();

		// Draw the vertical lines
		glBegin(GL_LINES);
		for (float x = 0; x <= 900.0; x += 100.0) {
			glVertex2f(x, 495);
			glVertex2f(x, 510.0);
		}
		for (float x = 0; x <= 900.0; x += 100.0) {
			glVertex2f(x, 0);
			glVertex2f(x, 10);
		}
		for (float x = 0; x <= 5.0; x += 5) {
			glVertex2f(x, 0);
			glVertex2f(x, 505);
		}
		for (float x = 895; x <= 900.0; x += 5) {
			glVertex2f(x, 0);
			glVertex2f(x, 505);
		}
		glEnd();
		glPopMatrix();
		/////////////////////////////////End Barrier & Obstacle Begin
		glPushMatrix();
		glColor3f(0.5, 0, 0);

		glBegin(GL_QUADS);
		glVertex2f(obs1x + 25, obs1y + 25);
		glVertex2f(obs1x - 25, obs1y + 25);
		glVertex2f(obs1x - 25, obs1y - 25);
		glVertex2f(obs1x + 25, obs1y - 25);
		glEnd();

		// Draw the 'X' inside the square
		glColor3f(0, 0, 0);

		glBegin(GL_LINES);
		glVertex2f(obs1x + 25, obs1y + 25);
		glVertex2f(obs1x - 25, obs1y - 25);

		glVertex2f(obs1x - 25, obs1y + 25);
		glVertex2f(obs1x + 25, obs1y - 25);
		glEnd();

		glColor3f(0.5, 0, 0);

		glBegin(GL_QUADS);
		glVertex2f(obs2x + 25, obs2y + 25);
		glVertex2f(obs2x - 25, obs2y + 25);
		glVertex2f(obs2x - 25, obs2y - 25);
		glVertex2f(obs2x + 25, obs2y - 25);
		glEnd();

		// Draw the 'X' inside the square
		glColor3f(0, 0, 0);

		glBegin(GL_LINES);
		glVertex2f(obs2x + 25, obs2y + 25);
		glVertex2f(obs2x - 25, obs2y - 25);

		glVertex2f(obs2x - 25, obs2y + 25);
		glVertex2f(obs2x + 25, obs2y - 25);
		glEnd();

		glColor3f(0.5, 0, 0);

		glBegin(GL_QUADS);
		glVertex2f(obs3x + 25, obs3y + 25);
		glVertex2f(obs3x - 25, obs3y + 25);
		glVertex2f(obs3x - 25, obs3y - 25);
		glVertex2f(obs3x + 25, obs3y - 25);
		glEnd();

		// Draw the 'X' inside the square
		glColor3f(0, 0, 0);

		glBegin(GL_LINES);
		glVertex2f(obs3x + 25, obs3y + 25);
		glVertex2f(obs3x - 25, obs3y - 25);

		glVertex2f(obs3x - 25, obs3y + 25);
		glVertex2f(obs3x + 25, obs3y - 25);
		glEnd();

		glColor3f(0.5, 0, 0);

		glBegin(GL_QUADS);
		glVertex2f(obs4x + 25, obs4y + 25);
		glVertex2f(obs4x - 25, obs4y + 25);
		glVertex2f(obs4x - 25, obs4y - 25);
		glVertex2f(obs4x + 25, obs4y - 25);
		glEnd();

		// Draw the 'X' inside the square
		glColor3f(0, 0, 0);

		glBegin(GL_LINES);
		glVertex2f(obs4x + 25, obs4y + 25);
		glVertex2f(obs4x - 25, obs4y - 25);

		glVertex2f(obs4x - 25, obs4y + 25);
		glVertex2f(obs4x + 25, obs4y - 25);
		glEnd();

		glPopMatrix();///////////////////////////Start Collectable & End Obstacle
		if (!collect1) {
			glPushMatrix();
			glColor3f(0.0f, 1.0f, 0.0f); // Green color
			glTranslated(collect1x, collect1y, 0);
			glRotated(collectRot, 0, 0, 1);
			glTranslated(-collect1x, -collect1y, 0);

			glBegin(GL_TRIANGLES);
			glVertex2f(collect1x, collect1y + 20); // Top vertex
			glVertex2f(collect1x - 20, collect1y); // Left vertex
			glVertex2f(collect1x, collect1y - 20); // Bottom vertex
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex2f(collect1x, collect1y + 20); // Top vertex
			glVertex2f(collect1x + 20, collect1y); // Right vertex
			glVertex2f(collect1x, collect1y - 20); // Bottom vertex
			glEnd();

			glColor3f(0.0f, 0.0f, 0.0f);
			glLineWidth(2);
			glBegin(GL_LINES);
			glVertex2f(collect1x - 20, collect1y);
			glVertex2f(collect1x + 20, collect1y);
			glEnd();
			glPopMatrix();
		}
		if (!collect2) {
			glPushMatrix();
			glColor3f(0.0f, 1.0f, 0.0f); // Green color
			glTranslated(collect2x, collect2y, 0);
			glRotated(collectRot, 0, 0, 1);
			glTranslated(-collect2x, -collect2y, 0);

			glBegin(GL_TRIANGLES);
			glVertex2f(collect2x, collect2y + 20); // Top vertex
			glVertex2f(collect2x - 20, collect2y); // Left vertex
			glVertex2f(collect2x, collect2y - 20); // Bottom vertex
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex2f(collect2x, collect2y + 20); // Top vertex
			glVertex2f(collect2x + 20, collect2y); // Right vertex
			glVertex2f(collect2x, collect2y - 20); // Bottom vertex
			glEnd();

			glColor3f(0.0f, 0.0f, 0.0f);
			glLineWidth(2);
			glBegin(GL_LINES);
			glVertex2f(collect2x - 20, collect2y);
			glVertex2f(collect2x + 20, collect2y);
			glEnd();
		}
		glPopMatrix();
		if (!collect3) {
			glPushMatrix();
			glColor3f(0.0f, 1.0f, 0.0f); // Green color
			glTranslated(collect3x, collect3y, 0);
			glRotated(collectRot, 0, 0, 1);
			glTranslated(-collect3x, -collect3y, 0);

			glBegin(GL_TRIANGLES);
			glVertex2f(collect3x, collect3y + 20); // Top vertex
			glVertex2f(collect3x - 20, collect3y); // Left vertex
			glVertex2f(collect3x, collect3y - 20); // Bottom vertex
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex2f(collect3x, collect3y + 20); // Top vertex
			glVertex2f(collect3x + 20, collect3y); // Right vertex
			glVertex2f(collect3x, collect3y - 20); // Bottom vertex
			glEnd();

			glColor3f(0.0f, 0.0f, 0.0f);
			glLineWidth(2);
			glBegin(GL_LINES);
			glVertex2f(collect3x - 20, collect3y);
			glVertex2f(collect3x + 20, collect3y);
			glEnd();
		}
		glPopMatrix();
		if (!collect4) {

			glPushMatrix();
			glColor3f(0.0f, 1.0f, 0.0f); // Green color
			glTranslated(collect4x, collect4y, 0);
			glRotated(collectRot, 0, 0, 1);
			glTranslated(-collect4x, -collect4y, 0);

			glBegin(GL_TRIANGLES);
			glVertex2f(collect4x, collect4y + 20); // Top vertex
			glVertex2f(collect4x - 20, collect4y); // Left vertex
			glVertex2f(collect4x, collect4y - 20); // Bottom vertex
			glEnd();

			glBegin(GL_TRIANGLES);
			glVertex2f(collect4x, collect4y + 20); // Top vertex
			glVertex2f(collect4x + 20, collect4y); // Right vertex
			glVertex2f(collect4x, collect4y - 20); // Bottom vertex
			glEnd();

			glColor3f(0.0f, 0.0f, 0.0f);
			glLineWidth(2);
			glBegin(GL_LINES);
			glVertex2f(collect4x - 20, collect4y);
			glVertex2f(collect4x + 20, collect4y);
			glEnd();
		}
		glPopMatrix();

		glPushMatrix();///////////////////////////////////////////Start PowerUp
		if (!power1) {
			glColor3f(0.0, 0.0, 1.0); // Set color to blue
			glTranslated(power1x, power1y, 0);
			glRotated(powerUpRot, 0, 0, 1);
			glTranslated(-power1x, -power1y, 0);
			// Draw the shaft of the arrow
			glBegin(GL_POLYGON);
			glVertex2f(power1x - 10, power1y - 15);
			glVertex2f(power1x + 10, power1y - 15);
			glVertex2f(power1x + 10, power1y + 5);
			glVertex2f(power1x - 10, power1y + 5);
			glEnd();

			// Draw the left head of the arrow
			glColor3f(0.0, 0.0, 1.0); // Set color to blue
			glBegin(GL_TRIANGLES);
			glVertex2f(power1x, power1y + 20);
			glVertex2f(power1x - 15, power1y);
			glVertex2f(power1x, power1y);
			glEnd();

			glColor3f(0.0, 0.0, 1.0); // Set color to blue
			// Draw the right head of the arrow
			glBegin(GL_TRIANGLES);
			glVertex2f(power1x, power1y + 20);
			glVertex2f(power1x + 15, power1y);
			glVertex2f(power1x, power1y);
			glEnd();

			glColor3f(0.0, 0.0, 1.0); // Set color to blue
			glBegin(GL_LINES);
			glVertex2f(power1x - 15, power1y);
			glVertex2f(power1x + 15, power1y);
			glEnd();
		}
		glPopMatrix();
		if (!power2) {
			glPushMatrix();
			glColor3f(1.0, 0.0, 1.0); // Set color to blue

			glTranslated(power2x, power2y, 0);
			glRotated(powerUpRot, 0, 0, 1);
			glTranslated(-power2x, -power2y, 0);

			// Draw the shaft of the arrow
			glBegin(GL_POLYGON);
			glVertex2f(power2x - 10, power2y - 15);
			glVertex2f(power2x + 10, power2y - 15);
			glVertex2f(power2x + 10, power2y + 5);
			glVertex2f(power2x - 10, power2y + 5);
			glEnd();

			// Draw the left head of the arrow
			glColor3f(1.0, 0.0, 1.0); // Set color to blue
			glBegin(GL_TRIANGLES);
			glVertex2f(power2x, power2y + 20);
			glVertex2f(power2x - 15, power2y);
			glVertex2f(power2x, power2y);
			glEnd();

			glColor3f(1.0, 0.0, 1.0); // Set color to blue
			// Draw the right head of the arrow
			glBegin(GL_TRIANGLES);
			glVertex2f(power2x, power2y + 20);
			glVertex2f(power2x + 15, power2y);
			glVertex2f(power2x, power2y);
			glEnd();

			glColor3f(1.0, 0.0, 1.0); // Set color to blue
			glBegin(GL_LINES);
			glVertex2f(power2x - 15, power2y);
			glVertex2f(power2x + 15, power2y);
			glEnd();
			glPopMatrix();
		}

		glPushMatrix();////////////////////////////////////////////////Player Start
		glTranslated(TransX, TransY, 0);
		glTranslated(450, 300, 0);
		glRotatef(RotZ, 0.0f, 0.0f, 1.0f);
		glTranslated(-450, -300, 0);

		glColor3f(1, 1, 1); // Light gray color

		glBegin(GL_QUADS);
		glVertex2f(445, 300);
		glVertex2f(455, 300);
		glVertex2f(455, 280);
		glVertex2f(445, 280);
		glEnd();

		// Draw the tip of the dart pin
		glColor3f(0.9f, 0.1f, 0.1f); // Red color

		glBegin(GL_TRIANGLES);
		glVertex2f(440, 300);
		glVertex2f(460, 300);
		glVertex2f(450, 320);
		glEnd();

		// Draw the decorative lines on the body of the dart pin
		glColor3f(0.0f, 0.0f, 0.0f); // Black color

		glBegin(GL_LINES);
		glVertex2f(445, 285);
		glVertex2f(455, 285);
		glEnd();

		// Draw a point at the tip of the dart pin
		glColor3f(1.0f, 1.0f, 1.0f); // Black color
		glPointSize(4.0f);
		glBegin(GL_POINTS);
		glVertex2f(450, 320);
		glEnd();
		glPopMatrix();
	}
	else
	{
		if (Won) {
			sprintf((char*)p0s, "Score: %d", Score);
			glPushMatrix();
			glColor3f(1, 0, 0);
			print(420, 550, (char*)p0s);
			glPopMatrix();

			sprintf((char*)p0s, "YOU WIN");
			glPushMatrix();
			glColor3f(0, 1, 0);
			print(400, 300, (char*)p0s);
			glPopMatrix();

		}
		else
		{
			sprintf((char*)p0s, "GAME OVER");
			glPushMatrix();
			glColor3f(1, 0, 0);
			print(400, 300, (char*)p0s);
			glPopMatrix();
		}
	}
	glFlush();
	glutSwapBuffers();
}

void Anim() {
	updateLines();

	if (powerUpRot <= 45.0f && powerUpDir == 1) {
		powerUpRot += 0.05f;
		if (powerUpRot >= 45) {
			powerUpDir = 0;
		}
	}
	if (powerUpRot >= -45.0f && powerUpDir == 0) {
		powerUpRot -= 0.05f;
		if (powerUpRot <= -45) {
			powerUpDir = 1;
		}
	}
	collectRot += 0.05f;
	if (collectRot >= 360.0f) {
		collectRot -= 360.0f;
	}

	if (targetScale <= 1.2 && targetDir == 1) {
		targetScale += 0.00005f;
		if (targetScale >= 1.2)
			targetDir = 0;
	}

	if (targetScale >= 1 && targetDir == 0) {
		targetScale -= 0.00005f;
		if (targetScale <= 1)
			targetDir = 1;
	}
}

void main(int argc, char** argr) {

	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(900, 600);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("Game");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 900, 0.0, 600);
	glutTimerFunc(0, time, 0);
	glutTimerFunc(0, power1time, 0);
	glutTimerFunc(0, power2time, 0);
	glutIdleFunc(Anim);
	glutSpecialFunc(Loc);
	glutDisplayFunc(Display);
	glutMainLoop();
}




