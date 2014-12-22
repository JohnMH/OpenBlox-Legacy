#ifndef OB_TYPE_CFRAME_H_
#define OB_TYPE_CFRAME_H_

#include "Vector3.h"

#define lua_cframe_name "luaL_CFrame"

namespace ob_type{
	class CFrame {
		public:
			CFrame();
			CFrame(double x, double y, double z);
			CFrame(double xx, double xy, double xz, double xw, double yx, double yy, double yz, double yw, double zx, double zy, double zz, double zw, double wx, double wy, double wz, double ww);
			virtual ~CFrame();

			double matrix[16];
			double* x;
			double* y;
			double* z;

			CFrame* inverse();
			std::string toString();

			Vector3* mul(Vector3* v);
			CFrame* mul(CFrame* m);
			CFrame* mul(double s);
			CFrame* add(Vector3* v);
			CFrame* sub(Vector3* v);
	};
};
#endif
