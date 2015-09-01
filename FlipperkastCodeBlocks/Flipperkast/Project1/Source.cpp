#include <GL/freeglut.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

const float PI = 4.0f*atanf(1.0f);
bool mouseDown = false;
bool leftFlip = false;
bool rightFlip=false;
float _anglex = 30.0f;
float _angley = 30.0f;
float _cameraAngle = 0.0f;
float _mousePosX;
float _mousePosY;
float counter = 0;
int rightTime=0;
int leftTime = 0;


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
		   return vector<float>{0.0f};
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

	void increment(Vector3 other){
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
	Vector3 operator +(Vector3 other){
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator -(Vector3 other){
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator *(float factor){
		return Vector3(x * factor, y * factor, z * factor);
	}
	float getLength(){
		return sqrtf(x * x + y * y + z * z);
	}
	public:float dot(Vector3 other){
		   return x * other.x + y * other.y + z * other.z;
	}
	public:Vector3 invert(){
		   return Vector3(-x, -y, -z);
	}
	public:void print(){
		   cout << "x= " << x << ",y= " << y << ",z= " << z << endl;
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
		vel.y -= 0.0005f;
	}
	void resetBall(){
		pos = identity;
	}
	void destroy(){
		destroyed = true;
	}
};
class Bumper{
public:
	float radius;
	Vector3 position;
	int colorIndex = 0;
	int cooldown = 500;
	vector<Vector3> color = vector<Vector3>{ Vector3(1, 1, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(0, 1, 1), Vector3(1, 0, 1), Vector3(1, 0, 0), Vector3(1, 1, 1), Vector3(0, 0, 0)};
	bool active;
	Bumper(){
		radius = 0;
		position = Vector3(0,0,0);
	}
	Bumper(float rad, Vector3 pos, bool act){
		active = act;
		radius = rad;
		position = pos;
	}
	public:void renderBumper(){
		glPushMatrix();
		glColor3f(color[colorIndex].x, color[colorIndex].y, color[colorIndex].z);
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(radius, 10, 10);
		glPopMatrix();
	}
};
vector<Bumper> bumpers = vector<Bumper>{
	Bumper(0.2f, Vector3(1.5, -1.3, 0.0), false),
	Bumper(0.2f, Vector3(-1.5, -1.3, 0.0), false),
	//Bumper(0.1f, Vector3(0,-1.8,0),false)
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

	return Vector3(-(calcVect1.y * calcVect2.z - calcVect1.z * calcVect2.y)
		, -(calcVect1.z * calcVect2.x - calcVect1.x * calcVect2.z)
		, -(calcVect1.x * calcVect2.y - calcVect1.y * calcVect2.x));

};
Vector3 normalizeVector(Vector3 vector){
	float length;
	length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	return Vector3(vector.x / length, vector.y / length, vector.z / length);
}
float randomFloat(){
	return (((float)rand() / RAND_MAX ) -0.5f)*2;
}
bool halt = false;
float calculateDegrees(Vector3 other, Vector3 normal){
	return acos(ball.vel.dot(normal) / (ball.vel.getLength() * normal.getLength())) * 180 / PI;
}
Vector3 calculatePointCollisionVectorToSphere(Ball ball, Vector3 centreSphere, float radius){
	//punt voor collision
	Vector3  previous = ball.pos-ball.vel;
	//vector tussen punt voor collision en midden van de bumper
	Vector3 centerToPrevious = centreSphere-(previous);
	//berekenen van verlengde van velocity ten opzichte van de vector tussen punt voor collision en midden van de bumper
	float dotproduct = centerToPrevious.dot(ball.vel) / ball.vel.dot(ball.vel);
	Vector3 component = ball.vel*dotproduct;
	//component van velocity ten opzichte van vector tussen punt voor collission en midden van de bumper loodrecht op voorgaande component
	Vector3 normalComponent = centerToPrevious - component;
	//lengte van de normaalcomponent
	float normalComponentLength = normalComponent.getLength();
	//lengte van velocity component dat binnen de sphere zit
	float lengthComponentPartly = sqrtf(radius * radius - normalComponentLength * normalComponentLength);
	// de ratio tussen component binnen sphere en de component in het geheel
	float ratio = lengthComponentPartly / component.getLength();
	//de vector die buiten de sphere staat
	Vector3 vectorFromPreviousToCollision = component*(1.0f - ratio);
	//uiteindelijke punt van collision
	Vector3 pointCollision = previous + (vectorFromPreviousToCollision);
	//vector vanaf het midden van de sphere naar de collision
	Vector3 centreToCollision = pointCollision - centreSphere;
	return centreSphere + (centreToCollision) ;
}
Vector3 calculateBounce(Vector3 other, Vector3 normal){
	float f = other.dot(normal);
	return  normal * (-2.0f * f) + other;
}
void handleKeypress(unsigned char key, int x, int y) {

	switch (key){
	case 27:
		exit(0);
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
class Flipper{
	public:
	Vector3 corner1;
	Vector3 corner2;
	Vector3 corner3;
	Vector3 corner4;
	Vector3 corner5;
	Vector3 corner6;
	Vector3 corner7;
	Vector3 corner8;
	Vector3 normalUp;
	Vector3 normalDown;
	Vector3 normalRight;
	Vector3 normalLeft;
	Vector3 normalFront;
	Vector3 normalBack;
	Vector3 rotation;

	public:Flipper(Vector3 point1, Vector3 point2){
				corner1 = point1;
				corner2 = Vector3(point1.x, point1.y, point2.z);
				corner3 = Vector3(point1.x, point2.y, point2.z);
				corner4 = Vector3(point1.x, point2.y, point1.z);
				corner5 = Vector3(point2.x, point1.y, point1.z);
				corner6 = Vector3(point2.x, point1.y, point2.z);
				corner7 = point2;
				corner8 = Vector3(point2.x, point2.y, point1.z);
				normalUp = Vector3(0.0f, 1.0f, 0.0f);
				normalDown = Vector3(0.0f, -1.0f, 0.0f);
				normalRight = Vector3(1.0f, 0.0f, 0.0f);
				normalLeft = Vector3(-1.0f, 0.0f, 0.0f);
				normalFront = Vector3(0.0f, 0.0f, 1.0f);
				normalBack = Vector3(0.0f, 0.0f, -1.0f);

	}
	public:Vector3 rotateAroundPoint(Vector3 rotationPoint, Vector3 degrees, Vector3 rotatedPoint){
			   rotatedPoint =rotatedPoint- (rotationPoint*2);
			   rotation.increment(degrees);
					Vector3 point = Vector3(
					  rotatedPoint.x * (cos(degrees.y) * cos(degrees.z))
					+ rotatedPoint.y * (cos(degrees.y) * sin(degrees.z))
					+ rotatedPoint.z * -sin(degrees.y)
					+ rotationPoint.x * (cos(degrees.y) * cos(degrees.z))
					+ rotationPoint.y * (cos(degrees.y) * sin(degrees.z))
					+ rotationPoint.z * -sin(degrees.y)
					- rotationPoint.x
					, rotatedPoint.x * ((sin(degrees.x) * sin(degrees.y) * cos(degrees.z)) + (cos(degrees.x)*-sin(degrees.z)))
					+ rotatedPoint.y * ((sin(degrees.x) * sin(degrees.y) * sin(degrees.z)) + (cos(degrees.x) * cos(degrees.z)))
					+ rotatedPoint.z * (sin(degrees.x) * cos(degrees.y))
					+ rotationPoint.x * ((sin(degrees.x) * sin(degrees.y) * cos(degrees.z)) + (cos(degrees.x)*-sin(degrees.z)))
					+ rotationPoint.y * ((sin(degrees.x) * sin(degrees.y) * sin(degrees.z)) + (cos(degrees.x) * cos(degrees.z)))
					+ rotationPoint.z * (sin(degrees.x) * cos(degrees.y))
					- rotationPoint.y
					, rotatedPoint.x * ((cos(degrees.x) * sin(degrees.y) * cos(degrees.z)) + (-sin(degrees.x) * -sin(degrees.z)))
					+ rotatedPoint.y * ((cos(degrees.x) * sin(degrees.y) * sin(degrees.z)) + (-sin(degrees.x) *cos(degrees.z)))
					+ rotatedPoint.z * (cos(degrees.x) * cos(degrees.y))
					+ rotationPoint.x * ((cos(degrees.x) * sin(degrees.y) * cos(degrees.z)) + (-sin(degrees.x) * -sin(degrees.z)))
					+ rotationPoint.y * ((cos(degrees.x) * sin(degrees.y) * sin(degrees.z)) + (-sin(degrees.x) *cos(degrees.z)))
					+ rotationPoint.z * (cos(degrees.x) * cos(degrees.y))
					-rotationPoint.z
					);
					point = point + (rotationPoint*2);
					return point;
	}
	public:void rotateFlipper(Vector3 pivot,Vector3 degrees){
		  corner1= rotateAroundPoint(pivot, degrees, corner1);
		  corner2 = rotateAroundPoint(pivot, degrees, corner2);
		  corner3 = rotateAroundPoint(pivot, degrees, corner3);
		  corner4 = rotateAroundPoint(pivot, degrees, corner4);
		  corner5 = rotateAroundPoint(pivot, degrees, corner5);
		  corner6 = rotateAroundPoint(pivot, degrees, corner6);
		  corner7 = rotateAroundPoint(pivot, degrees, corner7);
		  corner8 = rotateAroundPoint(pivot, degrees, corner8);
};
	public:void identity(){
		//Vector3(-rotation.x, -rotation.y, -rotation.z); gebruiken om vector terug te draaien
		//rotation = Vector3(0.0f,0.0f,0.0f);
	}
	public:void render(){
		glBegin(GL_QUADS);
		glNormal3f(normalFront.x, normalFront.y, normalFront.z);
		glVertex3f(corner6.x, corner6.y, corner6.z);
		glVertex3f(corner7.x, corner7.y, corner7.z);
		glVertex3f(corner3.x, corner3.y, corner3.z);
		glVertex3f(corner2.x, corner2.y, corner2.z);

		glNormal3f(normalBack.x, normalBack.y, normalBack.z);
		glVertex3f(corner1.x, corner1.y, corner1.z);
		glVertex3f(corner4.x, corner4.y, corner4.z);
		glVertex3f(corner8.x, corner8.y, corner8.z);
		glVertex3f(corner5.x, corner5.y, corner5.z);

		glNormal3f(normalDown.x, normalDown.y, normalDown.z);
		glVertex3f(corner5.x, corner5.y, corner5.z);
		glVertex3f(corner6.x, corner6.y, corner6.z);
		glVertex3f(corner2.x, corner2.y, corner2.z);
		glVertex3f(corner1.x, corner1.y, corner1.z);

		glNormal3f(normalUp.x, normalUp.y, normalUp.z);
		glVertex3f(corner4.x, corner4.y, corner4.z);
		glVertex3f(corner3.x, corner3.y, corner3.z);
		glVertex3f(corner7.x, corner7.y, corner7.z);
		glVertex3f(corner8.x, corner8.y, corner8.z);

		glNormal3f(normalLeft.x, normalLeft.y, normalLeft.z);
		glVertex3f(corner1.x, corner1.y, corner1.z);
		glVertex3f(corner2.x, corner2.y, corner2.z);
		glVertex3f(corner3.x, corner3.y, corner3.z);
		glVertex3f(corner4.x, corner4.y, corner4.z);

		glNormal3f(normalRight.x, normalRight.y, normalRight.z);
		glVertex3f(corner8.x, corner8.y, corner8.z);
		glVertex3f(corner7.x, corner7.y, corner7.z);
		glVertex3f(corner6.x, corner6.y, corner6.z);
		glVertex3f(corner5.x, corner5.y, corner5.z);
		glEnd();
	}
};
//top layer
Vector3 A = Vector3(-2.0f, 3.0f, -0.2f);
Vector3 B = Vector3(-2.0f, 3.0f, 0.2f);
Vector3 C = Vector3(2.0f, 3.0f, 0.2f);
Vector3 D = Vector3(2.0f, 3.0f, -0.2f);
//bottom layer
Vector3 H = Vector3(-2.0f, -1.5f, -0.2f);
Vector3 I = Vector3(-2.0f, -1.5f, 0.2f);
Vector3 J = Vector3(2.0f, -1.5f, 0.2f);
Vector3 K = Vector3(2.0f, -1.5f, -0.2f);
// bottom bottom layer
Vector3 P = Vector3(-1.0f, -2.25f, -0.2f);
Vector3 Q = Vector3(-1.0f, -2.25f, 0.2f);
Vector3 R = Vector3(1.0f, -2.25f, 0.2f);
Vector3 S = Vector3(1.0f, -2.25f, -0.2f);

Vector3 normalPQIH(normalizeVector(getNormal(P, Q, I)));
Vector3 normalQRJI(normalizeVector(getNormal(Q, R, J)));
Vector3 normalRSKJ(normalizeVector(getNormal(R, S, K)));
Vector3 normalSPHK(normalizeVector(getNormal(S, P, H)));
Flipper dolfijn = Flipper(Vector3(-1.5f, -1.4f, -0.2f), Vector3(-0.1f, -1.2f, 0.2f));
Flipper orca = Flipper(Vector3(0.1f, -1.4f, -0.2f), Vector3(1.5f, -1.2f, 0.2f));

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
vector<Vector3> createAABB(Flipper flip){
	float xmin = flip.corner1.x;
	float ymin = flip.corner1.y;
	float zmin = flip.corner1.z;
	float xmax = flip.corner1.x;
	float ymax = flip.corner1.y;
	float zmax = flip.corner1.z;
	vector<Vector3> corners = { flip.corner1, flip.corner2, flip.corner3, flip.corner4, flip.corner5, flip.corner6, flip.corner7, flip.corner8 };
	for (int i = 0; i < corners.size(); i++){

		if (corners[i].x < xmin){
			xmin = corners[i].x;
		}
		if (corners[i].y < ymin){
			ymin = corners[i].y;
		}
		if (corners[i].z < zmin){
			zmin = corners[i].z;
		}
		if (corners[i].x > xmax){
			xmax = corners[i].x;
		}
		if (corners[i].y > ymax){
			ymax = corners[i].y;
		}
		if (corners[i].z > zmax){
			zmax = corners[i].z;
		}
	}
	return{ Vector3(xmin, ymin, zmin), Vector3(xmax, ymax, zmax) };
}
vector<Vector3> createAABBVlak(Vector3 pos1, Vector3 pos2, Vector3 pos3, Vector3 pos4){

	Vector3 max = pos1;
	Vector3 min = pos1;

	if (min.x > pos2.x){
		min.x = pos2.x;
	}
	if (max.x < pos2.x){
		max.x = pos2.x;
	}
	if (min.x > pos3.x){
		min.x = pos3.x;
	}
	if (max.x < pos3.x){
		max.x = pos3.x;
	}
	if (min.x > pos4.x){
		min.x = pos4.x;
	}
	if (max.x < pos4.x){
		max.x = pos4.x;
	}

	return{ min, max };

}
Vector3 calculatePointCollisionVectorToFlipper(Ball ball, Flipper flipper,Vector3 normal, int plane){

	Vector3 middle = (flipper.corner7 + flipper.corner1)* 0.5;
	//float height = Vector3(0, flipper.corner7 - flipper.corner1), 0).getLength();
	float height = (flipper.corner7 - flipper.corner6).getLength();
	float width = (flipper.corner7 - flipper.corner3).getLength();
	Vector3 middleToBall = ball.pos - middle;
	Vector3 temp;
	float ratio;

	normal.print();
	switch(plane){
		case 0:
			temp = normal * (middleToBall.dot(normal) / normal.dot(normal));
			ratio = height /(2 * temp.getLength());
			cout << "top " << endl;
			break;//top plane of Flipper

		case 1:
			temp = normal * (middleToBall.dot(normal) / normal.dot(normal));
			ratio = width / (2 * temp.getLength());
			cout << "right side" << endl;
			break;//right side plane of Flipper
		case 2:
			temp = normal * (middleToBall.dot(normal) / normal.dot(normal));
			ratio = height / (2 * temp.getLength());
			cout << "bottom" << endl;
			break;//bottom plane of Flipper
		case 3:
			temp = normal * (middleToBall.dot(normal) / normal.dot(normal));
			ratio = width / (2 * temp.getLength());
			cout << "left side" << endl;
			break;//left side plane of Flipper
	}
	return middle + (middleToBall * ratio);
}
bool checkCollision(){
	vector<Vector3> boundingBoxDolfijn = createAABB(dolfijn);
	//if (ball.pos.x > boundingBoxDolfijn[0].x && ball.pos.y > boundingBoxDolfijn[0].y && ball.pos.z > boundingBoxDolfijn[0].z
	//&&	ball.pos.x < boundingBoxDolfijn[1].x && ball.pos.y < boundingBoxDolfijn[1].y && ball.pos.z < boundingBoxDolfijn[1].z){

	//if (getLambda(ball.pos, getConstante(getNormal(dolfijn.corner5, dolfijn.corner2, dolfijn.corner1), dolfijn.corner1), getNormal(dolfijn.corner5, dolfijn.corner2, dolfijn.corner1)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(dolfijn.corner5,dolfijn.corner2,dolfijn.corner1)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, dolfijn, getNormal(dolfijn.corner5, dolfijn.corner2, dolfijn.corner1),0);
	//}
	//if (getLambda(ball.pos, getConstante(getNormal(dolfijn.corner1, dolfijn.corner2, dolfijn.corner3), dolfijn.corner1), getNormal(dolfijn.corner1, dolfijn.corner2, dolfijn.corner3)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(dolfijn.corner1, dolfijn.corner2, dolfijn.corner3)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, dolfijn, getNormal(dolfijn.corner1, dolfijn.corner2, dolfijn.corner3),1);
	//}
	//if (getLambda(ball.pos, getConstante(getNormal(dolfijn.corner7, dolfijn.corner3, dolfijn.corner4), dolfijn.corner7), getNormal(dolfijn.corner7, dolfijn.corner3, dolfijn.corner4)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(dolfijn.corner7, dolfijn.corner3, dolfijn.corner4)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, dolfijn, getNormal(dolfijn.corner5, dolfijn.corner3, dolfijn.corner4),2);
	//}
	//if (getLambda(ball.pos, getConstante(getNormal(dolfijn.corner7, dolfijn.corner6, dolfijn.corner5), dolfijn.corner7), getNormal(dolfijn.corner7, dolfijn.corner6, dolfijn.corner5)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(dolfijn.corner7, dolfijn.corner6, dolfijn.corner5)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, dolfijn, getNormal(dolfijn.corner7, dolfijn.corner6, dolfijn.corner5), 3);
	//}
	//}
	//vector<Vector3> boundingBoxOrca = createAABB(orca);
	//if (ball.pos.x > boundingBoxOrca[0].x && ball.pos.y > boundingBoxOrca[0].y && ball.pos.z > boundingBoxOrca[0].z
	//&&	ball.pos.x < boundingBoxOrca[1].x && ball.pos.y < boundingBoxOrca[1].y && ball.pos.z < boundingBoxOrca[1].z){
	//if (getLambda(ball.pos, getConstante(getNormal(orca.corner5, orca.corner2, orca.corner1), orca.corner1), getNormal(orca.corner5, orca.corner2, orca.corner1)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(orca.corner5, orca.corner2, orca.corner1)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, orca, getNormal(orca.corner5, orca.corner2, orca.corner1), 0);
	//}
	//if (getLambda(ball.pos, getConstante(getNormal(orca.corner1, orca.corner2, orca.corner3), orca.corner1), getNormal(orca.corner1, orca.corner2, orca.corner3)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(orca.corner1, orca.corner2, orca.corner3)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, orca, getNormal(orca.corner1, orca.corner2, orca.corner3), 1);
	//}
	//if (getLambda(ball.pos, getConstante(getNormal(orca.corner7, orca.corner3, orca.corner4), orca.corner7), getNormal(orca.corner7, orca.corner3, orca.corner4)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(orca.corner7, orca.corner3, orca.corner4)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, orca, getNormal(orca.corner7, orca.corner3, orca.corner4), 2);
	//}
	//if (getLambda(ball.pos, getConstante(getNormal(orca.corner7, orca.corner6, orca.corner5), orca.corner7), getNormal(orca.corner7, orca.corner6, orca.corner5)) > 0){
	//ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(orca.corner7, orca.corner6, orca.corner5)));
	//ball.pos = calculatePointCollisionVectorToFlipper(ball, orca, getNormal(orca.corner7, orca.corner6, orca.corner5), 3);
	//}
	//}
	vector<Vector3> dolfijnAABB = createAABBVlak(dolfijn.corner1, dolfijn.corner2, dolfijn.corner5, dolfijn.corner6);
	if (ball.pos.x > dolfijnAABB[0].x && ball.pos.x < dolfijnAABB[1].x && ball.pos.y > dolfijnAABB[0].y && ball.pos.y < dolfijnAABB[1].y && ball.pos.z > dolfijnAABB[0].z && ball.pos.z < dolfijnAABB[1].z){
		dolfijnAABB[0].print();
		dolfijnAABB[1].print();
		if (getLambda(ball.pos, getConstante(getNormal(dolfijn.corner5, dolfijn.corner2, dolfijn.corner1), dolfijn.corner1), getNormal(dolfijn.corner5, dolfijn.corner2, dolfijn.corner1)) > 0){
			ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(dolfijn.corner7, dolfijn.corner6, dolfijn.corner5)));
		}
	}
	vector<Vector3> orcaAABB = createAABBVlak(orca.corner1, orca.corner2, orca.corner5, orca.corner6);
	cout << (ball.pos.x > orcaAABB[0].x && ball.pos.x < orcaAABB[1].x) << endl;
	orcaAABB[1].print();
	if (ball.pos.x > orcaAABB[0].x && ball.pos.x < orcaAABB[1].x && ball.pos.y > orcaAABB[0].y && ball.pos.y < orcaAABB[1].y && ball.pos.z > orcaAABB[0].z && ball.pos.z < orcaAABB[1].z){

		if (getLambda(ball.pos, getConstante(getNormal(orca.corner5, orca.corner2, orca.corner1), orca.corner1), getNormal(orca.corner5, orca.corner2, orca.corner1)) > 0){
			ball.vel = calculateBounce(ball.vel, normalizeVector(getNormal(orca.corner7, orca.corner6, orca.corner5)));
		}
	}

	if (ball.pos.x > C.x - ball.rad || ball.pos.x < A.x + ball.rad){
		ball.vel.multiply(0.99f);
		ball.vel.x *= -1;

	}
	if (ball.pos.y  > C.y - ball.rad){
		ball.vel.multiply(0.99f);
		ball.vel.y *= -1;

	}
	if (ball.pos.z > C.z - ball.rad || ball.pos.z < A.z + ball.rad){
		ball.vel.multiply(0.99f);
		ball.vel.z *= -1;

	}
	if (ball.pos.y < P.y ){
		ball.destroy();
	}
	if (getLambda(ball.pos, getConstante(getNormal(P, Q, I), P), getNormal(P, Q, I)) > 0){
		ball.vel = calculateBounce(ball.vel, normalPQIH);
		ball.vel.multiply(0.95f);
	}
	if (getLambda(ball.pos, getConstante(getNormal(Q, R, J), Q), getNormal(Q, R, J)) > 0){
		ball.vel = calculateBounce(ball.vel, normalQRJI);
		ball.vel.multiply(0.95f);
	}
	if (getLambda(ball.pos, getConstante(getNormal(P, S, K), P), getNormal(P, S, K)) < 0){
		ball.vel = calculateBounce(ball.vel, normalSPHK);
		ball.vel.multiply(0.95f);
	}
	if (getLambda(ball.pos, getConstante(getNormal(S, R, J), S), getNormal(S, R, J)) < 0){
		ball.vel = calculateBounce(ball.vel, normalRSKJ);
		ball.vel.multiply(0.95f);
	}
	for (int i = 0; i < bumpers.size(); i++){
		if ((ball.pos - bumpers[i].position).getLength() < bumpers[i].radius){
			ball.vel = calculateBounce(ball.vel, normalizeVector(calculatePointCollisionVectorToSphere(ball, bumpers[i].position, bumpers[i].radius) - bumpers[i].position));
			ball.pos = calculatePointCollisionVectorToSphere(ball, bumpers[i].position, bumpers[i].radius);
			if (bumpers[i].active&&bumpers[i].colorIndex<bumpers[i].color.size()){
				ball.vel *(1.3f + (bumpers[i].colorIndex / 10));
				bumpers[i].colorIndex++;
			}

		}
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

	for (int i = 0; i < bumpers.size(); i++){
		bumpers[i].renderBumper();
		glColor3f(1.0f, 1.0f, 0.0f);
	}

	dolfijn.render();
	orca.render();

	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0f);
	glVertex3f(B.x, B.y, B.z);
	glVertex3f(C.x, C.y, C.z);
	glVertex3f(J.x, J.y, J.z);
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

	//PQIH
	glNormal3f(normalPQIH.x, normalPQIH.y, normalPQIH.z);
	glVertex3f(H.x, H.y, H.z);
	glVertex3f(I.x, I.y, I.z);
	glVertex3f(Q.x, Q.y, Q.z);
	glVertex3f(P.x, P.y, P.z);

	//QRJI
	glNormal3f(normalQRJI.x, normalQRJI.y, normalQRJI.z);
	glVertex3f(I.x, I.y, I.z);
	glVertex3f(J.x, J.y, J.z);
	glVertex3f(R.x, R.y, R.z);
	glVertex3f(Q.x, Q.y, Q.z);

	//PSKH
	glNormal3f(normalSPHK.x, normalSPHK.y, normalSPHK.z);
	glVertex3f(P.x, P.y, P.z);
	glVertex3f(S.x, S.y, S.z);
	glVertex3f(K.x, K.y, K.z);
	glVertex3f(H.x, H.y, H.z);

	//SRJK
	glNormal3f(normalRSKJ.x, normalRSKJ.y, normalRSKJ.z);
	glVertex3f(S.x, S.y, S.z);
	glVertex3f(R.x, R.y, R.z);
	glVertex3f(J.x, J.y, J.z);
	glVertex3f(K.x, K.y, K.z);

	glEnd();
	glutSwapBuffers();
}

void checkFlipper(){

	if (!leftFlip&&leftTime>0){
		dolfijn.rotateFlipper(dolfijn.corner1 - (dolfijn.corner1 - dolfijn.corner3)*0.5, Vector3(0.0f, 0.0f, 0.05f));
		leftTime--;
	}
	else if (leftFlip &&leftTime<15){
		dolfijn.rotateFlipper(dolfijn.corner1 - (dolfijn.corner1 - dolfijn.corner3)*0.5, Vector3(0.0f, 0.0f, -0.05f));
		leftTime++;
	}
	if (!rightFlip&& rightTime>0){
		orca.rotateFlipper(orca.corner5 - (orca.corner5 - orca.corner7)*0.5, Vector3(0.0f, 0.0f, -0.05f));
		rightTime--;
	}
	else if (rightFlip && rightTime <15){
		orca.rotateFlipper(orca.corner5 - (orca.corner5 - orca.corner7)*0.5, Vector3(0.0f, 0.0f, 0.05f));
		rightTime++;
	}


	if (GetAsyncKeyState(65) == 0){

		leftFlip = false;
	}
	else{
		leftFlip = true;
	}
	if (GetAsyncKeyState(68) == 0){

		rightFlip = false;
	}
	else{

		rightFlip = true;
	}
}
void update(int value){
		if (!halt){
			if (!ball.destroyed){
				ball.update();
				checkCollision();
			}
			checkFlipper();
			for (int i = 0; i < bumpers.size(); i++)
			{
				if (bumpers[i].colorIndex>0){
					bumpers[i].cooldown--;
					cout << "cooling down " << i << ", its now at " << bumpers[i].cooldown << endl;
					if (bumpers[i].cooldown < 1){
						bumpers[i].cooldown = 100;
						cout << "color degraded to " << bumpers[i].colorIndex - 1 << endl;
						bumpers[i].colorIndex--;
					}
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, update, 0);
	}

void mouse(int button, int state, int x, int y){

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !mouseDown){
		_mousePosX = LOWORD(x);
		_mousePosY = LOWORD(y);
		mouseDown = true;
	}

	//Kijken of de linker muisknop is losgelaten
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
	{
		mouseDown = false;
	}
}

//als Muisknop is ingedrukt, check coordinaten van de muis en zet de rotatie van de kubus erop om te draaien
void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		_anglex += (LOWORD(y) - _mousePosY);
		_angley += (LOWORD(x) - _mousePosX);
		_mousePosX = LOWORD(x);
		_mousePosY = LOWORD(y);
	}
}
int main(int argc, char **argv) {
	srand((unsigned)time(NULL));

	dolfijn.rotateFlipper(dolfijn.corner1 - (dolfijn.corner1 - dolfijn.corner3)*0.5, Vector3(0.0f, 0.0f, 0.5f));
	orca.rotateFlipper(orca.corner5 - (orca.corner5 - orca.corner7)*0.5, Vector3(0.0f, 0.0f, -0.5f));

	Vector3 position(0, 0, 0);
	Vector3 velocity(randomFloat() / 50, (randomFloat()) / 50, 0);
	ball = Ball(position, velocity, 0.1f);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Basic Shapes");
	initRendering();
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(0, update, 0);
	glutMainLoop();
	return 0;
}
