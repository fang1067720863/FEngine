#include"Geometry.h"
//
//void ShapeGeometryBuilder::build(const FBox& box)
//{
//	float dx = box.mHalfLength;
//	float dy = box.mHalfWidth;
//	float dz = box.mHalfHeight;
//
//	// 24 points in box
//	// create body
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(-dx, -dy, dz);
//
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(-dx, -dy, -dz);
//
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(dx, -dy, -dz);
//
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(dx, -dy, dz);
//
//	// +ve y plane
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(dx, dy, dz);
//
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(dx, dy, -dz);
//
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(-dx, dy, -dz);
//
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(-dx, dy, dz);
//
//	// +ve x plane
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(dx, -dy, dz);
//
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(dx, -dy, -dz);
//
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(dx, dy, -dz);
//
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(dx, dy, dz);
//
//	// -ve x plane
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(-dx, dy, dz);
//
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(-dx, dy, -dz);
//
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(-dx, -dy, -dz);
//
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(-dx, -dy, dz);
//
//	//createTop
//	// +ve z plane
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(-dx, dy, dz);
//
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(-dx, -dy, dz);
//
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(dx, -dy, dz);
//
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(dx, dy, dz);
//
//
//	//createBottom
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(dx, dy, -dz);
//
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(dx, -dy, -dz);
//
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(-dx, -dy, -dz);
//
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(-dx, dy, -dz);
//
//
//}

//void ShapeGeometryBuilder::build(const FBox& box)
//{
//	float dx = box.mHalfLength;
//	float dy = box.mHalfWidth;
//	float dz = box.mHalfHeight;
//	
//	// 24 points in box
//	// create body
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(-dx, -dy, dz);
//	
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(-dx, -dy, -dz);
//	
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(dx, -dy, -dz);
//	
//	Normal3f(0.0f, -1.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(dx, -dy, dz);
//	
//	// +ve y plane
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(dx, dy, dz);
//	
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(dx, dy, -dz);
//	
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(-dx, dy, -dz);
//	
//	Normal3f(0.0f, 1.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(-dx, dy, dz);
//	
//	// +ve x plane
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(dx, -dy, dz);
//	
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(dx, -dy, -dz);
//	
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(dx, dy, -dz);
//	
//	Normal3f(1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(dx, dy, dz);
//	
//	// -ve x plane
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(-dx, dy, dz);
//	
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(-dx, dy, -dz);
//	
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(-dx, -dy, -dz);
//	
//	Normal3f(-1.0f, 0.0f, 0.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(-dx, -dy, dz);
//	
//	//createTop
//	// +ve z plane
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(-dx, dy, dz);
//	
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(-dx, -dy, dz);
//	
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(dx, -dy, dz);
//	
//	Normal3f(0.0f, 0.0f, 1.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(dx, dy, dz);
//	
//	
//	//createBottom
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(0.0f, 1.0f);
//	Vertex3f(dx, dy, -dz);
//	
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(0.0f, 0.0f);
//	Vertex3f(dx, -dy, -dz);
//	
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(1.0f, 0.0f);
//	Vertex3f(-dx, -dy, -dz);
//	
//	Normal3f(0.0f, 0.0f, -1.0f);
//	TexCoord2f(1.0f, 1.0f);
//	Vertex3f(-dx, dy, -dz);
//	
//}
