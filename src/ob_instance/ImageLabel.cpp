#include "ImageLabel.h"

#ifndef OPENBLOX_SERVER
	#include "../openblox/OpenBloxRenderUtil.h"
#endif

namespace ob_instance{
	DEFINE_CLASS(ImageLabel, true, false);

	ImageLabel::ImageLabel() : GuiLabel(){
		Name = ClassName;

		ImageTransparency = 0;
		ImageRectSize = new ob_type::Vector2(0, 0);
		ImageRectOffset = new ob_type::Vector2(0, 0);
		ImageColor3 = new ob_type::Color3(1, 1, 1);
		Image = "";

		tex_ID = 0;
		needsReload = false;
	}

	ImageLabel::~ImageLabel(){}

	void ImageLabel::setImage(std::string newImage){
		if(newImage != Image){
			Image = newImage;

			needsReload = true;

			std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
			args.push_back(ob_type::VarWrapper("Image"));

			Changed->Fire(args);
		}
	}

	Instance* ImageLabel::cloneImpl(){
		ImageLabel* newGuy = new ImageLabel;
		newGuy->Name = Name;

		newGuy->Active = Active;
		newGuy->BackgroundColor3 = BackgroundColor3->clone();
		newGuy->BackgroundTransparency = BackgroundTransparency;
		newGuy->BorderColor3 = BorderColor3;
		newGuy->BorderSizePixel = BorderSizePixel;
		newGuy->ClipsDescendants = ClipsDescendants;
		newGuy->Draggable = Draggable;
		newGuy->Position = Position->clone();
		newGuy->Rotation = Rotation;
		newGuy->Size = Size->clone();
		newGuy->Visible = Visible;
		newGuy->ZIndex = ZIndex;

		newGuy->ImageTransparency = 0;
		newGuy->ImageRectSize = ImageRectSize->clone();
		newGuy->ImageRectOffset = ImageRectOffset->clone();
		newGuy->ImageColor3 = ImageColor3->clone();
		newGuy->Image = Image;

		return newGuy;
	}

	void ImageLabel::render(){
		if(needsReload){
			OpenBlox::asset_response_body* arb = OpenBlox::AssetLocator::getAsset(Image);
			if(arb != NULL){
				tex_ID = SOIL_load_OGL_texture_from_memory((unsigned char*)arb->data, arb->size, SOIL_LOAD_AUTO, tex_ID, SOIL_FLAG_MIPMAPS);
			}else{
				tex_ID = 0;
			}
			needsReload = false;
		}
		#ifndef OPENBLOX_SERVER
		if(Visible){
			double halfX = AbsolutePosition->x + (AbsoluteSize->x/2);
			double halfY = AbsolutePosition->y + (AbsoluteSize->y/2);
			glTranslated(halfX, halfY, 0);
			glRotated(Rotation, 0, 0, 1);
			glTranslated(-halfX, -halfY, 0);

			glColor4d(BackgroundColor3->r, BackgroundColor3->g, BackgroundColor3->b, 1 - BackgroundTransparency);

			const GLdouble dubindex = ZIndex;

			const GLdouble vertices[]{
				AbsolutePosition->x,  AbsolutePosition->y + AbsoluteSize->y, dubindex, //Bottom Left
				AbsolutePosition->x,  AbsolutePosition->y, dubindex, //Top Left
				AbsolutePosition->x + AbsoluteSize->x,  AbsolutePosition->y, dubindex, //Top Right
				AbsolutePosition->x + AbsoluteSize->x,  AbsolutePosition->y + AbsoluteSize->y, dubindex //Bottom Right
			};
			const GLubyte indices[]{
				0, 1, 2,//Bottom Left, Top Left, Top Right
				0, 2, 3//Bottom Left, Top Right, Bottom Right
			};

			GLfloat texture[]{
				0, 1,
				0, 0,
				1, 0,
				1, 1
			};

			double oX = ImageRectOffset->x;
			double oY = ImageRectOffset->y;
			double sX = ImageRectSize->x;
			double sY = ImageRectSize->y;
			GLdouble texture_set[]{
				oX, oY + sY,
				oX, oY,
				oX + sX, oY,
				oX + sX, oY + sY
			};

			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_DOUBLE, 0, vertices);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

			if(tex_ID != 0){
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnable(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, tex_ID);

				glColor4d(ImageColor3->r, ImageColor3->g, ImageColor3->b, 1 - ImageTransparency);

				glVertexPointer(3, GL_DOUBLE, 0, vertices);
				if(ImageRectSize->x == 0 && ImageRectSize->y == 0 && ImageRectOffset->x == 0 && ImageRectOffset->y == 0){
					glTexCoordPointer(2, GL_FLOAT, 0, texture);
				}else{
					glTexCoordPointer(2, GL_DOUBLE, 0, texture_set);
				}
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

				glDisable(GL_TEXTURE_2D);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			if(BorderSizePixel > 0 && BackgroundTransparency < 1){
				glColor4d(BorderColor3->r, BorderColor3->g, BorderColor3->b, 1 - BackgroundTransparency);

				const GLubyte lines[]{
					0, 1,//Bottom Left, Top Left
					1, 2,//Top Left, Top Right
					2, 3,//Top Right, Bottom Right
					3, 0//Bottom Right, Bottom Left
				};

				glLineWidth(BorderSizePixel);

				glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, lines);
			}

			glDisableClientState(GL_VERTEX_ARRAY);

			for(std::vector<Instance*>::size_type i = 0; i != children.size(); i++){
				Instance* kid = children[i];
				if(kid != NULL){
					if(GuiObject* go = dynamic_cast<GuiObject*>(kid)){
						go->render();
					}
				}
			}

			glTranslated(halfX, halfY, 0);
			glRotated(-Rotation, 0, 0, 1);
			glTranslated(-halfX, -halfY, 0);
		}
		#endif
	}

	void ImageLabel::register_lua_property_getters(lua_State* L){
		luaL_Reg props[]{
			{"Image", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					lua_pushstring(L, il->Image.c_str());
					return 1;
				}
				return 0;
			}},
			{"ImageColor3", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					return il->ImageColor3->wrap_lua(L);
				}
				return 0;
			}},
			{"ImageRectSize", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					return il->ImageRectSize->wrap_lua(L);
				}
				return 0;
			}},
			{"ImageRectOffset", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					return il->ImageRectOffset->wrap_lua(L);
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		GuiObject::register_lua_property_getters(L);
	}

	void ImageLabel::register_lua_property_setters(lua_State* L){
		luaL_Reg props[]{
			{"Image", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					std::string newImage = std::string(luaL_checkstring(L, 2));
					il->setImage(newImage);
					return 0;
				}
				return 0;
			}},
			{"ImageColor3", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					ob_type::Color3* newVal = ob_type::checkColor3(L, 2);
					if(il->ImageColor3 != newVal){
						il->ImageColor3 = newVal;

						std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
						args.push_back(ob_type::VarWrapper("ImageColor3"));

						il->Changed->Fire(args);
					}
				}
				return 0;
			}},
			{"ImageRectSize", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					ob_type::Vector2* newVal = ob_type::checkVector2(L, 2);
					if(il->ImageRectSize != newVal){
						il->ImageRectSize = newVal;

						std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
						args.push_back(ob_type::VarWrapper("ImageRectSize"));

						il->Changed->Fire(args);
					}
				}
				return 0;
			}},
			{"ImageRectOffset", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(ImageLabel* il = dynamic_cast<ImageLabel*>(inst)){
					ob_type::Vector2* newVal = ob_type::checkVector2(L, 2);
					if(il->ImageRectOffset != newVal){
						il->ImageRectOffset = newVal;

						std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
						args.push_back(ob_type::VarWrapper("ImageRectOffset"));

						il->Changed->Fire(args);
					}
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		GuiObject::register_lua_property_setters(L);
	}
}
