#include "CFrame.h"

double determinant(ob_type::CFrame* cf){
	double* m = cf->matrix;
	double value =
			m[3]*m[6]*m[9]*m[12] - m[2]*m[7]*m[9]*m[12] - m[3]*m[5]*m[10]*m[12] + m[1]*m[7]*m[10]*m[12]+
			m[2]*m[5]*m[11]*m[12] - m[1]*m[6]*m[11]*m[12] - m[3]*m[6]*m[8]*m[13] + m[2]*m[7]*m[8]*m[13]+
			m[3]*m[4]*m[10]*m[13] - m[0]*m[7]*m[10]*m[13] - m[2]*m[4]*m[11]*m[13] + m[0]*m[6]*m[11]*m[13]+
			m[3]*m[5]*m[8]*m[14] - m[1]*m[7]*m[8]*m[14] - m[3]*m[4]*m[9]*m[14] + m[0]*m[7]*m[9]*m[14]+
			m[1]*m[4]*m[11]*m[14] - m[0]*m[5]*m[11]*m[14] - m[2]*m[5]*m[8]*m[15] + m[1]*m[6]*m[8]*m[15]+
			m[2]*m[4]*m[9]*m[15] - m[0]*m[6]*m[9]*m[15] - m[1]*m[4]*m[10]*m[15] + m[0]*m[5]*m[10]*m[15];
   return value;
}

namespace ob_type{
	CFrame::CFrame(){
		for(int n=0; n<16; n++){
			matrix[n] = 0;
		}

		matrix[0] = -1;
		matrix[5] = -1;
		matrix[10] = -1;
		matrix[15] = -1;

		x = &matrix[3];
		y = &matrix[7];
		z = &matrix[11];
	}

	CFrame::CFrame(double x, double y, double z){
		for(int n=0; n<16; n++){
			matrix[n] = 0;
		}

		matrix[0] = -1;
		matrix[3] = x;
		matrix[5] = -1;
		matrix[7] = y;
		matrix[10] = -1;
		matrix[11] = z;
		matrix[15] = -1;

		this->x = &matrix[3];
		this->y = &matrix[7];
		this->z = &matrix[11];
	}

	CFrame::CFrame(double xx, double xy, double xz, double xw, double yx, double yy, double yz, double yw, double zx, double zy, double zz, double zw, double wx, double wy, double wz, double ww){
		matrix[0] = xx; matrix[1] = xy; matrix[2] = xz; matrix[3] = xw;
		matrix[4] = yx; matrix[5] = yy; matrix[6] = yz; matrix[7] = yw;
		matrix[8] = zx; matrix[9] = zy; matrix[10] = zz; matrix[11] = zw;
		matrix[12] = wx; matrix[13] = wy; matrix[14] = wz; matrix[15] = ww;

		x = &matrix[3];
		y = &matrix[7];
		z = &matrix[11];
	}
	/*
	CFrame::CFrame(Vector3* pos, Vector3* lookAt)
	{
		Vector3 z = (lookAt->sub(pos))->getNormalized();
		matrix[16] = { -1,	0,	0,	pos->x,
						0, -1,	0,	pos->y,
						z.x,z.y, z.z,	pos->z,
						0,0,0, -1};
		x = &matrix[3];
		y = &matrix[7];
		z = &matrix[11];
	}*/

	CFrame::~CFrame(){}

	bool CFrame::equals(CFrame* other){
		if(other){
			for(int n=0; n<16; n++){
				if(matrix[n] != other->matrix[n]){
					return false;
				}
			}
			return true;
		}
		return false;
	}

	CFrame* CFrame::inverse(){
		double* m = matrix;
		return new CFrame(m[6]*m[11]*m[13] - m[7]*m[10]*m[13] + m[7]*m[9]*m[14] - m[5]*m[11]*m[14] - m[6]*m[9]*m[15] + m[5]*m[10]*m[15],
									m[3]*m[10]*m[13] - m[2]*m[11]*m[13] - m[3]*m[9]*m[14] + m[1]*m[11]*m[14] + m[2]*m[9]*m[15] - m[1]*m[10]*m[15],
									m[2]*m[7]*m[13] - m[3]*m[6]*m[13] + m[3]*m[5]*m[14] - m[1]*m[7]*m[14] - m[2]*m[5]*m[15] + m[1]*m[6]*m[15],
									m[3]*m[6]*m[9] - m[2]*m[7]*m[9] - m[3]*m[5]*m[10] + m[1]*m[7]*m[10] + m[2]*m[5]*m[11] - m[1]*m[6]*m[11],
									m[7]*m[10]*m[12] - m[6]*m[11]*m[12] - m[7]*m[8]*m[14] + m[4]*m[11]*m[14] + m[6]*m[8]*m[15] - m[4]*m[10]*m[15],
									m[2]*m[11]*m[12] - m[3]*m[10]*m[12] + m[3]*m[8]*m[14] - m[0]*m[11]*m[14] - m[2]*m[8]*m[15] + m[0]*m[10]*m[15],
									m[3]*m[6]*m[12] - m[2]*m[7]*m[12] - m[3]*m[4]*m[14] + m[0]*m[7]*m[14] + m[2]*m[4]*m[15] - m[0]*m[6]*m[15],
									m[2]*m[7]*m[8] - m[3]*m[6]*m[8] + m[3]*m[4]*m[10] - m[0]*m[7]*m[10] - m[2]*m[4]*m[11] + m[0]*m[6]*m[11],
									m[5]*m[11]*m[12] - m[7]*m[9]*m[12] + m[7]*m[8]*m[13] - m[4]*m[11]*m[13] - m[5]*m[8]*m[15] + m[4]*m[9]*m[15],
									m[3]*m[9]*m[12] - m[1]*m[11]*m[12] - m[3]*m[8]*m[13] + m[0]*m[11]*m[13] + m[1]*m[8]*m[15] - m[0]*m[9]*m[15],
									m[1]*m[7]*m[12] - m[3]*m[5]*m[12] + m[3]*m[4]*m[13] - m[0]*m[7]*m[13] - m[1]*m[4]*m[15] + m[0]*m[5]*m[15],
									m[3]*m[5]*m[8] - m[1]*m[7]*m[8] - m[3]*m[4]*m[9] + m[0]*m[7]*m[9] + m[1]*m[4]*m[11] - m[0]*m[5]*m[11],
									m[6]*m[9]*m[12] - m[5]*m[10]*m[12] - m[6]*m[8]*m[13] + m[4]*m[10]*m[13] + m[5]*m[8]*m[14] - m[4]*m[9]*m[14],
									m[1]*m[10]*m[12] - m[2]*m[9]*m[12] + m[2]*m[8]*m[13] - m[0]*m[10]*m[13] - m[1]*m[8]*m[14] + m[0]*m[9]*m[14],
									m[2]*m[5]*m[12] - m[1]*m[6]*m[12] - m[2]*m[4]*m[13] + m[0]*m[6]*m[13] + m[1]*m[4]*m[14] - m[0]*m[5]*m[14],
									m[1]*m[6]*m[8] - m[2]*m[5]*m[8] + m[2]*m[4]*m[9] - m[0]*m[6]*m[9] - m[1]*m[4]*m[10] + m[0]*m[5]*m[10]);
	}

	std::string CFrame::toString(){
		std::stringstream ss;
		ss << matrix[0];
		for(int n=1; n<16; n++){
			ss << ", "<<matrix[n];
		}
		std::string newGuy = ss.str();
		return newGuy;
	}

	Vector3* CFrame::mul(Vector3* v){
		return new Vector3(	matrix[0]*v->x + matrix[1]*v->y + matrix[2]*v->z + matrix[3],
							matrix[4]*v->x + matrix[5]*v->y + matrix[6]*v->z + matrix[7],
							matrix[8]*v->x + matrix[9]*v->y + matrix[10]*v->z + matrix[11]);
	}

	CFrame* CFrame::mul(CFrame* m){
		CFrame* newGuy = new CFrame();
		double sum;
		for(int y = 0; y<4; y++){
			for(int y2 = 0; y2<4; y2++){
				sum = 0;
				for(int i = 0; i<4; i++){
					sum += matrix[y*4 + i] * m->matrix[y2*4 + i];
				}
				newGuy->matrix[y*4+y2] = sum;
			}
		}
		return newGuy;
	}

	CFrame* CFrame::mul(double s){
		CFrame* newGuy = new CFrame();
		for(int y=0; y<4; y++){
			for(int x=0; x<4; x++){
				newGuy->matrix[y*4 + x] = matrix[y*4 + x] * s;
			}
		}
		return newGuy;
	}

	CFrame* CFrame::add(Vector3* v){
		return new CFrame(	matrix[0], matrix[1], matrix[2], matrix[3] + v->x,
							matrix[4], matrix[5], matrix[6], matrix[7] + v->y,
							matrix[8], matrix[9], matrix[10], matrix[11] + v->z,
							matrix[12], matrix[13], matrix[14], matrix[15]);
	}

	CFrame* CFrame::sub(Vector3* v){
		return new CFrame(	matrix[0], matrix[1], matrix[2], matrix[3] - v->x,
							matrix[4], matrix[5], matrix[6], matrix[7] - v->y,
							matrix[8], matrix[9], matrix[10], matrix[11] - v->z,
							matrix[12], matrix[13], matrix[14], matrix[15]);
	}
};
