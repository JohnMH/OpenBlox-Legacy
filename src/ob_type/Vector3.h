#ifndef OB_TYPE_VECTOR3_H_
#define OB_TYPE_VECTOR3_H_

#include "../openblox/OpenBlox.h"
#include <cmath>

namespace ob_instance{
	class Vector3{
		public:
			Vector3();
			Vector3(double x, double y, double z);
			virtual ~Vector3();

			double x;
			double y;
			double z;
			double magnitude;

			Vector3* getNormalized();//unit

			Vector3* add(double v);
			Vector3* add(Vector3* other);
			Vector3* sub(double v);
			Vector3* sub(Vector3* other);
			Vector3* mul(double v);
			Vector3* mul(Vector3* other);
			Vector3* div(double v);
			Vector3* div(Vector3* other);
			Vector3* neg();

			Vector3* lerp(Vector3* goal, double alpha);
			double dot(Vector3* other);
			Vector3* cross(Vector3* other);
			bool isClose(Vector3* other, double epsilon = 1e-6);
	};

}
#endif
