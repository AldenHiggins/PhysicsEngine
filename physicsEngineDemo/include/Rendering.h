#ifndef RENDERING_H
#define RENDERING_H

namespace PhysicsDemo
{
	class Rendering 
	{
	public:
		static void drawSphere(Vector3 position, Vector3 color, real radius);

		static void drawBox(GLfloat mat[16], Vector3 color, Vector3 halfSizes);
	
	
	};




}


#endif // RENDERING_H
