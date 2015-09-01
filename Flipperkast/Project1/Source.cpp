#include <GL/freeglut.h> 
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

const double PI = 4.0*atan(1.0);
bool mouseDown = false;
float _anglex = 30.0f;
float _angley = 30.0f;
float _cameraAngle = 0.0f;


class Matrix{
public:vector<float> translatePoint(vector<float> point, vector<float> translation){
		   point[0] += (float)translation[0];
		   point[1] += (float)translation[1];
		   point[2] += (float)translation[2];
		   return point;
}
public:vector<float> rotatePointXYZ(vector<float> point, float xDegrees, float yDegrees, float zDegrees){
		   //returns a point after being turned in the rotation matrix(alot of work)
		   return{
			   point[0] * (cos(yDegrees) * cos(zDegrees))
			   + point[1] * (cos(yDegrees) * sin(zDegrees))
			   + point[2] * -sin(yDegrees)
			   , point[0] * ((sin(xDegrees) * sin(yDegrees) * cos(zDegrees)) + (cos(xDegrees)*-sin(zDegrees)))
			   + point[1] * ((sin(xDegrees) * sin(yDegrees) * sin(zDegrees)) + (cos(xDegrees) * cos(zDegrees)))
			   + point[2] * (sin(xDegrees) * cos(yDegrees))
			   , point[0] * ((cos(xDegrees) * sin(yDegrees) * cos(zDegrees)) + (-sin(xDegrees) * -sin(zDegrees)))
			   + point[1] * ((cos(xDegrees) * sin(yDegrees) * sin(zDegrees)) + (-sin(xDegrees) *cos(zDegrees)))
			   + point[2] * (cos(xDegrees) * cos(yDegrees))
		   };
}
};

Matrix matrix = Matrix();
class Vector3{
public:
	float x, y, z;
	Vector3(){
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float xVal, float yVal, float zVal){
		x = xVal;
		y = yVal;
		z = zVal;
	}

	void add(Vector3 other){
		x += other.x;
		y += other.y;
		z += other.z;
	}
	void subtract(Vector3 other){
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	void multiply(Vector3 other){
		x *= other.x;
		y *= other.y;
		z *= other.z;
	}
	void multiply(float num){
		x *= num;
		y *= num;
		z *= num;
	}
	float getLength(){
		return sqrtf(x * x + y * y + z * z);
	}
public:float dot(Vector3 other){
		   return x * other.x + y * other.y + z * other.z;
}
public:void Invert(){
		   x = -x;
		   y = -y;
		   z = -z;

}

};

class Cube{
public:
	vector<vector<float>> identity;
	vector<vector<float>> currentState;
	Cube(){
	}

	Cube(vector<vector<float>> points){
		identity = points;
		currentState = points;
	}
	void revertCube(){
		currentState = identity;
	}
	void rotateCube(float x, float y, float z){
		currentState[0] = matrix.rotatePointXYZ(currentState[0], x, y, z);
		currentState[1] = matrix.rotatePointXYZ(currentState[1], x, y, z);
		currentState[2] = matrix.rotatePointXYZ(currentState[2], x, y, z);
		currentState[3] = matrix.rotatePointXYZ(currentState[3], x, y, z);
		currentState[4] = matrix.rotatePointXYZ(currentState[4], x, y, z);
		currentState[5] = matrix.rotatePointXYZ(currentState[5], x, y, z);
		currentState[6] = matrix.rotatePointXYZ(currentState[6], x, y, z);
		currentState[7] = matrix.rotatePointXYZ(currentState[7], x, y, z);
		//rotating normal vectors
		currentState[8] = matrix.rotatePointXYZ(currentState[8], x, y, z);
		currentState[9] = matrix.rotatePointXYZ(currentState[9], x, y, z);
		currentState[10] = matrix.rotatePointXYZ(currentState[10], x, y, z);
		currentState[11] = matrix.rotatePointXYZ(currentState[11], x, y, z);
		currentState[12] = matrix.rotatePointXYZ(currentState[12], x, y, z);
		currentState[13] = matrix.rotatePointXYZ(currentState[13], x, y, z);
	}
};
class Ball{
public:
	Vector3 pos;
	Vector3 identity;
	Vector3 vel;
	bool destroyed = false;
	float rad;
public:
	Ball(){
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
	}
	Ball(Vector3 position, Vector3 velocity, float radius){
		pos = position;
		identity = position;
		vel = velocity;
		rad = radius;
	}
	void update(){
		pos.x += vel.x;
		pos.y += vel.y;
		pos.z += vel.z;
		vel.y -= 0.0005;
	}
	void resetBall(){
		pos = identity;
	}
	void destroy(){
		destroyed = true;
	}
};
Ball ball = Ball();
Vector3 getNormal(Vector3 A, Vector3 B, Vector3 C)
{
	Vector3 calcVect1, calcVect2;
	calcVect1.x = B.x - A.x;
	calcVect1.y = B.y - A.y;
	calcVect1.z = B.z - A.z;

	calcVect2.x = C.x - A.x;
	calcVect2.y = C.y - A.y;
	calcVect2.z = C.z - A.z;

	//float vectorNormL =(float) sqrt(calcVect1.x * calcVect2.x + calcVect1.y + calcVect2.y + calcVect1.z + calcVect2.z);
	//cout << vectorNormL << endl;
	return Vector3(-(calcVect1.y * calcVect2.z - calcVect1.z * calcVect2.y)
		, -(calcVect1.z * calcVect2.x - calcVect1.x * calcVect2.z)
		, -(calcVect1.x * calcVect2.y - calcVect1.y * calcVect2.x));

};
Vector3 normalizeVector(Vector3 vector){
	float length;
	length = (float)sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	return Vector3(vector.x / length, vector.y / length, vector.z / length);
}
float randomFloat(){
	return (float)rand() / RAND_MAX + 1;
}
bool halt = false;
float calculateDegrees(Vector3 other, Vector3 normal){
	cout << acos(ball.vel.dot(normal) / (ball.vel.getLength() * normal.getLength()));
	return (float)acos(ball.vel.dot(normal) / (ball.vel.getLength() * normal.getLength())) * 180 / PI;
}
Vector3 calculateReflect(Vector3 other, Vector3 normal){
	Vector3 vec = normal;
	//2 * (V dot N)*N - V
	float f = -2 * other.dot(normal);
	vec.multiply(f);
	vec.add(other);
	return vec;
}
void handleKeypress(unsigned char key, int x, int y) {

	switch (key){
	case 27:
		exit(0);
		break;
	case 97:
		_angley -= 1.5f;
		break;
	case 100:
		_angley += 1.5f;
		break;
	case 119:
		_anglex -= 1.5f;
		break;
	case 115:
		_anglex += 1.5f;
		break;
	case 32:
		if (halt == false){
			halt = true;
		}
		else{
			halt = false;
		}
	}

	if (_anglex > 360){
		_anglex -= 360.0f;
	}
	if (_anglex < 0){
		_anglex += 360.0f;
	}
	if (_angley > 360){
		_angley -= 360.0f;
	}
	if (_angley < 0){
		_angley += 360.0f;
	}
	//  Request display update
	glutPostRedisplay();
}
void initRendering(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_NORMALIZE);

	//glClearColor(0.7f,0.9f,1.0f,1.0f);
}
void handleResize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);

}
//top layer
Vector3 A = Vector3(-1.5f, 1.5f, -1.5f);
Vector3 B = Vector3(-1.5f, 1.5f, 1.5f);
Vector3 C = Vector3(1.5f, 1.5f, 1.5f);
Vector3 D = Vector3(1.5f, 1.5f, -1.5f);
//bottom layer
Vector3 H = Vector3(-1.5f, -1.5f, -1.5f);
Vector3 I = Vector3(-1.5f, -1.5f, 1.5f);
Vector3 J = Vector3(1.5f, -1.5f, 1.5f);
Vector3 K = Vector3(1.5f, -1.5f, -1.5f);
// bottom bottom layer
Vector3 P = Vector3(-0.75f, -2.25f, -0.75f);
Vector3 Q = Vector3(-0.75f, -2.25f, 0.75f);
Vector3 R = Vector3(0.75f, -2.25f, 0.75f);
Vector3 S = Vector3(0.75f, -2.25f, -0.75f);

Vector3 normalPQIH(normalizeVector(getNormal(P, Q, I)));
Vector3 normalQRJI(normalizeVector(getNormal(Q, R, J)));
Vector3 normalRSKJ(normalizeVector(getNormal(R, S, K)));
Vector3 normalSPHK(normalizeVector(getNormal(S, P, H)));

float getConstante(Vector3 LcalcvectorNorm, Vector3 Point)
{
	float Vlakx;
	float Vlaky;
	float Vlakz;
	Vlakx = LcalcvectorNorm.x * Point.x;
	Vlaky = LcalcvectorNorm.y * Point.y;
	Vlakz = LcalcvectorNorm.z * Point.z;
	return Vlakx + Vlaky + Vlakz;
};
float getLambda(Vector3 Ballpos, float Constante, Vector3 Normal)
{
	return ((Normal.x*Ballpos.x + Normal.y*Ballpos.y + Normal.z*Ballpos.z - Constante) / (Normal.x * Normal.x + Normal.y * Normal.y + Normal.z * Normal.z) * -1);
};

bool checkCollision(){
	if (ball.pos.x > 1.5 - ball.rad || ball.pos.x < -1.5 + ball.rad){
		ball.vel.multiply(0.99);
		ball.vel.x *= -1;
		
	}
	if (ball.pos.y  > 1.5 - ball.rad){
		ball.vel.multiply(0.99);
		ball.vel.y *= -1;
		
	}
	if (ball.pos.z > 1.5 - ball.rad || ball.pos.z < -1.5 + ball.rad){
		ball.vel.multiply(0.99);
		ball.vel.z *= -1;
		
	}
	ball.vel.multiply(0.9999);
	if (ball.pos.y < -2.25 ){
		ball.destroy();
	}
	//cout << getNormal(P, Q, I).x << " , " << getNormal(P, Q, I).y << " , " << getNormal(P, Q, I).z << endl;
	//cout << getNormal(R, S, K).x << " , " << getNormal(R, S, K).y << " , " << getNormal(R, S, K).z << endl;
	if (getLambda(ball.pos, getConstante(getNormal(P, Q, I), P), getNormal(P, Q, I)) > 0){
		std::cout << true + 1;
		ball.vel = calculateReflect(ball.vel, normalPQIH);
		ball.vel.multiply(0.99);
	}
	if (getLambda(ball.pos, getConstante(getNormal(Q, R, J), Q), getNormal(Q, R, J)) > 0){
		std::cout << true + 2;
		ball.vel = calculateReflect(ball.vel, normalQRJI);
		ball.vel.multiply(0.99);
	}
	if (getLambda(ball.pos, getConstante(getNormal(P, S, K), P), getNormal(P, S, K)) < 0){
		std::cout << true + 3;
		ball.vel = calculateReflect(ball.vel, normalSPHK);
		ball.vel.multiply(0.99);
	}
	if (getLambda(ball.pos, getConstante(getNormal(S, R, J), S), getNormal(S, R, J)) < 0){
		std::cout << true + 4;
		ball.vel = calculateReflect(ball.vel, normalRSKJ);
		ball.vel.multiply(0.99);
	}
	return false;
}

void drawScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);

	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f };
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glRotatef(_angley, 0.0f, 1.0f, 0.0f);
	glRotatef(_anglex, 1.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	if (!ball.destroyed){
		glPushMatrix();
		glTranslatef(ball.pos.x, ball.pos.y, ball.pos.z);
		glutSolidSphere(ball.rad, 10, 10);
		glPopMatrix();
	}
	glBegin(GL_QUADS);
	//cout << normalPQIH.x << " " << normalPQIH.y << " " << normalPQIH.z << endl;


	glNormal3f(0.0, 0.0, -1.0f);
	glVertex3f(B.x, B.y, B.z);
	glVertex3f(C.x, C.y, C.z);
	glVertex3f(J.x, J.y, J.x);
	glVertex3f(I.x, I.y, I.z);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(J.x, J.y, J.z);
	glVertex3f(C.x, C.y, C.z);
	glVertex3f(D.x, D.y, D.z);
	glVertex3f(K.x, K.y, K.z);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(K.x, K.y, K.z);
	glVertex3f(D.x, D.y, D.z);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(H.x, H.y, H.z);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(B.x, B.y, B.z);
	glVertex3f(I.x, I.y, I.z);
	glVertex3f(H.x, H.y, H.z);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(A.x, A.y, A.z);
	glVertex3f(D.x, D.y, D.z);
	glVertex3f(C.x, C.y, C.z);
	glVertex3f(B.x, B.y, B.z);
	/*
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(I.x, I.y, I.z);
	glVertex3f(J.x, J.y, J.z);
	glVertex3f(K.x, K.y, K.z);
	glVertex3f(H.x, H.y, H.z);
	*/
	//Vector3 A = Vector3(-1.5f, 1.5f, -1.5f);
	//Vector3 B = Vector3(-1.5f, 1.5f, 1.5f);
	//Vector3 C = Vector3(1.5f, 1.5f, 1.5f);
	//Vector3 D = Vector3(1.5f, 1.5f, -1.5f);
	//bottom layer
	//Vector3 H = Vector3(-1.5f, -1.5f, -1.5f);
	//Vector3 I = Vector3(-1.5f, -1.5f, 1.5f);
	//Vector3 J = Vector3(1.5f, -1.5f, 1.5f);
	//Vector3 K = Vector3(1.5f, -1.5f, -1.5f);
	// bottom bottom layer
	//Vector3 P = Vector3(-0.75f, -2.25f, -0.75f);
	//Vector3 Q = Vector3(-0.75f, -2.25f, 0.75f);
	//Vector3 R = Vector3(0.75f, -2.25f, 0.75f);
	//Vector3 S = Vector3(0.75f, -2.25f, -0.75f);
	//PQIH
	//cout << normalPQIH.x << endl;
	glNormal3f(normalPQIH.x, normalPQIH.y, normalPQIH.z);
	glVertex3f(H.x, H.y, H.z);
	glVertex3f(I.x, I.y, I.z);
	glVertex3f(Q.x, Q.y, Q.z);
	glVertex3f(P.x, P.y, P.z);


	//QRJI(Q, R, J);
	glNormal3f(normalQRJI.x, normalQRJI.y, normalQRJI.z);
	glVertex3f(I.x, I.y, I.z);
	glVertex3f(J.x, J.y, J.z);
	glVertex3f(R.x, R.y, R.z);
	glVertex3f(Q.x, Q.y, Q.z);




	//PSKH(P, S, K);
	glNormal3f(normalRSKJ.x, normalRSKJ.y, normalRSKJ.z);
	glVertex3f(P.x, P.y, P.z);
	glVertex3f(S.x, S.y, S.z);
	glVertex3f(K.x, K.y, K.z);
	glVertex3f(H.x, H.y, H.z);

	//SRJK(S, R, J);
	//glNormal3f(normalSPHK.x, normalSPHK.y, normalSPHK.z);
	glVertex3f(S.x, S.y, S.z);
	glVertex3f(R.x, R.y, R.z);
	glVertex3f(J.x, J.y, J.z);
	glVertex3f(K.x, K.y, K.z);

	glEnd();


	glutSwapBuffers();

}

void update(int value){
	//cube.revertCube();
	//cube.rotateCube(_anglex, _angley, 0);
	while (halt){

	}
	float temp = getLambda(ball.pos, getConstante(getNormal(P, Q, I), Q), getNormal(P, Q, I));

	if (!ball.destroyed){
		//cout << getNormal(P, Q, I).x  * temp << endl;
		ball.update();
		checkCollision();
	}
	glutPostRedisplay();
	glutTimerFunc(30, update, 0);
}
void mouse(int button, int state, int x, int y){

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		mouseDown = true;
	}

	//Kijken of de linker muisknop is losgelaten
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	{
		mouseDown = false;
	}
}//als Muisknop is ingedrukt, check coordinaten van de muis en zet de rotatie van de kubus erop om te draaien
void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		_anglex = LOWORD(y);
		_angley = LOWORD(x);
	}
}
int main(int argc, char **argv) {

	srand((unsigned int)time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	glutCreateWindow("Basic Shapes");
	initRendering();

	//Vector3 normalQRJI(getNormal());
	//Vector3 normalRSKJ(getNormal());
	//Vector3 normalSPHK(getNormal());
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	Vector3 position(0, 0, 0);
	Vector3 velocity(randomFloat() / 100, randomFloat() / 100, randomFloat() / 100);
	ball = Ball(position, velocity, 0.1f);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(0, update, 0);
	glutMainLoop();
	return 0;
}
