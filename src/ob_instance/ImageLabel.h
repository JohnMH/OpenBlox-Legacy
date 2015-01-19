#ifndef OB_INSTANCE_IMAGELABEL_H_
#define OB_INSTANCE_IMAGELABEL_H_

#include "GuiLabel.h"

namespace ob_instance{
	class ImageLabel: public GuiLabel{
		public:
			ImageLabel();
			virtual ~ImageLabel();

			void setImage(std::string newImage);

			virtual void render();

			DECLARE_CLASS(ImageLabel);

			bool needsReload;
			GLuint tex_ID;

			std::string Image;
			ob_type::Color3* ImageColor3;
			ob_type::Vector2* ImageRectOffset;
			ob_type::Vector2* ImageRectSize;
			float ImageTransparency;

			static void register_lua_property_setters(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
	};
}
#endif
