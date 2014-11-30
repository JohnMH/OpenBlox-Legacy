#include "Vector3.h"

namespace ob_instance{
	Vector3::Vector3(){
		x = 0;
		y = 0;
		z = 0;
		magnitude = 1;
	}

	Vector3::Vector3(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
		magnitude = sqrt(dot(this));
	}

	Vector3::~Vector3(){}

	Vector3* Vector3::getNormalized(){
		Vector3* newGuy = new Vector3();
		newGuy->x = x;
		newGuy->y = y;
		newGuy->z = z;
		return newGuy;
	}

	Vector3* Vector3::add(double v){
		return new Vector3(x + v, y + v, z + v);
	}

	Vector3* Vector3::add(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x + other->x, y + other->y, z + other->z);
	}

	Vector3* Vector3::sub(double v){
		return new Vector3(x - v, y - v, z - v);
	}

	Vector3* Vector3::sub(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x - other->x, y - other->y, z - other->z);
	}

	Vector3* Vector3::mul(double v){
		return new Vector3(x * v, y * v, z * v);
	}

	Vector3* Vector3::mul(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x * other->x, y * other->y, z * other->z);
	}

	Vector3* Vector3::div(double v){
		return new Vector3(x / v, y / v, z / v);
	}

	Vector3* Vector3::div(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(x / other->x, y / other->y, z / other->z);
	}

	Vector3* Vector3::neg(){
		return new Vector3(-x, -y, -z);
	}

	Vector3* Vector3::lerp(Vector3* goal, double alpha){
		if(goal == NULL){
			return NULL;
		}
		return add(alpha)->mul(goal->sub(this));
	}

	double Vector3::dot(Vector3* other){
		if(other == NULL){
			return 0;
		}
		return x*other->x + y*other->y + z*other->z;
	}

	Vector3* Vector3::cross(Vector3* other){
		if(other == NULL){
			return NULL;
		}
		return new Vector3(y*other->z - z*other->y, x*other->z - other->z*x, x*other->y - other->y*x);
	}

	bool Vector3::isClose(Vector3* other, double epsilon){
		if(other == NULL){
			return NULL;
		}
		return sub(other)->magnitude <= epsilon;
	}
}
