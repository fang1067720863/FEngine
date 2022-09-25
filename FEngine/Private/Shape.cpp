
#include"Shape.h"
#include"Geometry.h"

bool ShapeGeometryBuilder::Draw(const FBox& box)
{
	Vertex3f(-1.0f, -1.0f, -1.0f);
	Normal3f(0.0f, -1.0f, 0.0f);
	TexCoord2f(0.0f, 0.0f);

	Vertex3f(-1.0f, 1.0f, -1.0f);
	Normal3f(0.0f, 1.0f, 0.0f);
	TexCoord2f(1.0f, 0.0f);


	Vertex3f(1.0f, 1.0f, -1.0f);
	Normal3f(1.0f, 0.0f, 0.0f);
	TexCoord2f(1.0f, 0.0f);

	Vertex3f(1.0f, -1.0f, -1.0f);
	Normal3f(1.0f, 0.0f, 0.0f);
	TexCoord2f(0.0f, 0.0f);

	Vertex3f(-1.0f, -1.0f, 1.0f);
	Normal3f(0.0f, -1.0f, 0.0f);
	TexCoord2f(0.0f, 1.0f);

	Vertex3f(-1.0f, 1.0f, 1.0f);
	Normal3f(-1.0f, 0.0f, 0.0f);
	TexCoord2f(0.0f, 1.0f);

	Vertex3f(1.0f, 1.0f, 1.0f);
	Normal3f(0.0f, 1.0f, 0.0f);
	TexCoord2f(0.0f, 1.0f);

	Vertex3f(1.0f, -1.0f, 1.0f);
	Normal3f(1.0f, 0.0f, 0.0f);
	TexCoord2f(0.0f, 1.0f);

	unsigned int indices[] = {
		// 正面
		0, 1, 2,
		2, 3, 0,
		// 左面
		4, 5, 1,
		1, 0, 4,
		// 顶面
		1, 5, 6,
		6, 2, 1,
		// 背面
		7, 6, 5,
		5, 4, 7,
		// 右面
		3, 2, 6,
		6, 7, 3,
		// 底面
		4, 0, 3,
		3, 7, 4
	};
	for (int i = 0; i < 36; i++)
	{
		mIndexArray->emplace_back(indices[i]);
	}
	return false;
}

bool ShapeGeometryBuilder::Draw(const FTriangle& triangle)
{
	Vertex3f(0.5f, 0.5f, 0.5f);
	Vertex3f(0.8f, 0.5f, 0.5f);
	Vertex3f(0.5f, -0.5f, 0.5f);
	return false;
}

bool ShapeGeometryBuilder::Draw(const FSphere& event)
{
	Vertex3f(-1.0f, 1.0f, 0.5f);  // left-top
	Vertex3f(1.0f, 1.0f, 0.5f);   // right-top
	Vertex3f(1.0f, -1.0f, 0.5f);  // right-bottom
	Vertex3f(-1.0f, -1.0f, 0.5f); // top-bottom
	Normal3f(0.0f, 0.0f, -1.0f);
	Normal3f(0.0f, 0.0f, -1.0f);
	Normal3f(0.0f, 0.0f, -1.0f);
	Normal3f(0.0f, 0.0f, -1.0f);
	TexCoord2f(0.0f, 0.0f);
	TexCoord2f(1.0f, 0.0f);
	TexCoord2f(1.0f, 1.0f);
	TexCoord2f(0.0f, 1.0f);
	// anti clock
	unsigned int indices[] = {
		// 正面
		0, 1, 2,
		0, 2, 3
	};
	for (int i = 0; i < 6; i++)
	{
		mIndexArray->emplace_back(indices[i]);
	}
	return false;
}

bool ShapeGeometryBuilder::Draw(const FQuad& quad)
{
	Vertex3f(-1.0f, 1.0f, 0.5f);  // left-top
	Vertex3f(1.0f, 1.0f, 0.5f);   // right-top
	Vertex3f(1.0f, -1.0f, 0.5f);  // right-bottom
	Vertex3f(-1.0f, -1.0f, 0.5f); // top-bottom
	Normal3f(0.0f, 0.0f, -1.0f);
	Normal3f(0.0f, 0.0f, -1.0f);
	Normal3f(0.0f, 0.0f, -1.0f);
	Normal3f(0.0f, 0.0f, -1.0f);
	TexCoord2f(0.0f, 0.0f);
	TexCoord2f(1.0f, 0.0f);
	TexCoord2f(1.0f, 1.0f);
	TexCoord2f(0.0f, 1.0f);
	// anti clock
	unsigned int indices[] = {
		// 正面
		0, 1, 2,
		0, 2, 3
	};
	for (int i = 0; i < 6; i++)
	{
		mIndexArray->emplace_back(indices[i]);
	}
	return false;
}


void ShapeGeometryBuilder::End()
{
	// Quad Mode
	for (unsigned int i = 0; i < _vertices->size(); i += 4)
	{
		unsigned int p0 = i;
		unsigned int p1 = i + 1;
		unsigned int p2 = i + 2;
		unsigned int p3 = i + 3;

		mIndexArray->emplace_back(p0);
		mIndexArray->emplace_back(p1);
		mIndexArray->emplace_back(p2);

		mIndexArray->emplace_back(p2);
		mIndexArray->emplace_back(p3);
		mIndexArray->emplace_back(p0);
	}
}
Ptr<FGeometry> ShapeGeometryBuilder::BuildGeomtry(const IShape& shape)
{
	shape.Drawn(*this);
	Ptr<FGeometry> geom = new FGeometry();
	geom->SetPositionArray(_vertices.get());
	geom->SetNormalArray(mNormalArray.get());
	geom->SetTexcoordArray(mTexcoordArray.get());
	geom->SetIndexArray(mIndexArray.get());
	geom->Compile();
	Clear();
	return geom;
}
