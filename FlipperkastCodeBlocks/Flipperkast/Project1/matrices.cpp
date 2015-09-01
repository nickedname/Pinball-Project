#pragma once
#include <math.h>
#include <vector>

using namespace std;

class Matrix{
	public:vector<float> translatePoint(vector<float> point, vector<float> translation){
			   point[0] += (float)translation[0];
			   point[1] += (float)translation[1];
			   point[2] += (float)translation[2];
			   return point;
	}
	public:vector<float> rotatePoint(vector<float> point, float xDegrees, float yDegrees, float zDegrees){
			   rotatePointX(point, xDegrees);
			   rotatePointY(point, yDegrees);
			   rotatePointZ(point, zDegrees);
			   return point;
	}
	public:vector<float> rotatePointX(vector<float> point, float degrees){
			   point[0] = (float)point[0];
			   point[1] = (float)(point[1] * cos(degrees) + point[2] * -sin(degrees));
			   point[2] = (float)(point[1] * sin(degrees) + point[2] * cos(degrees));
			   return point;
	}
	public:vector<float> rotatePointY(vector<float> point, float degrees){
			   point[0] = (float)(point[0] * cos(degrees) + point[2] * sin(degrees));
			   point[1] = (float)point[1];
			   point[2] = (float)(point[0] * -sin(degrees) + point[2] * cos(degrees));
			   return point;
	}
	public:vector<float> rotatePointZ(vector<float> point, float degrees){
			   point[0] = point[0] * cos(degrees) + point[1] * -sin(degrees);
			   point[1] = point[0] * sin(degrees) + point[1] * cos(degrees);
			   point[2] = point[2];
			   return point;
	}

};
