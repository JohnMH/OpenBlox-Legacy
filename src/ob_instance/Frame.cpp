#include "Frame.h"

namespace ob_instance{
	DEFINE_CLASS(Frame, true, false);

	Frame::Frame() : GuiObject(){
		Name = ClassName;
	}

	Frame::~Frame(){}

	Instance* Frame::cloneImpl(){
		Frame* newGuy = new Frame;
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

		return newGuy;
	}

	void Frame::render(){
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
			const GLubyte lines[]{
				0, 1,//Bottom Left, Top Left
				1, 2,//Top Left, Top Right
				2, 3,//Top Right, Bottom Right
				3, 0//Bottom Right, Bottom Left
			};

			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_DOUBLE, 0, vertices);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

			glColor4d(BorderColor3->r, BorderColor3->g, BorderColor3->b, 1 - BackgroundTransparency);

			glLineWidth(BorderSizePixel);

			glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, lines);

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
}
