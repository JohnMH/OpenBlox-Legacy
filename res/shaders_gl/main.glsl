uniform mat4 viewMatrix, projMatrix;

in vec4 position;

void main(){
	gl_Postion = projMatrix * viewMatrix * position;
}