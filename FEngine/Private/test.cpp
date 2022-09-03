
#include<vector>
#include<iostream>
#include"ShaderParameter.h"
#include"Geometry.h"
#include"UtfConverter.h"
#include"GltfReader.h"
#include<wrl/client.h>
#include<DirectXMath.h>
#include"Camera.h"
using namespace DirectX;

class Apple:public FReference {

public:
	Apple() { val = "123"; std::cout << " Apple default ctor" << std::endl; }
	Apple(const Apple& a) { val = a.val;  std::cout << " Apple copy ctor" << std::endl; }
	Apple& operator= (const Apple& a) { val = 1; std::cout << "Apple copy assgin" << std::endl; return *this; }
	// a.val 类本身有移动构造会调用自己的移动构造 如string会移动走，int 没有移动构造就调用拷贝构造 
	Apple(Apple&& a) :val(std::move(a.val)) { std::cout << "Apple move ctor" << std::endl; }      // 一般移动是继续调用移动 ，即std::move(a.val)  但 move(int) 好像还是拷贝
	Apple& operator= (Apple&& a)noexcept { std::cout << "Apple move assgin" << std::endl; return *this; }
	~Apple() { std::cout << "Apple dctor" << std::endl; }
	inline string Get() { return val; }
	void Set(string v) { val = v; }
private:
	string val;
};

void TestPtr()
{
	Ptr<Apple> apple = new Apple();
	std::cout << "----------------------------------" << std::endl;
	apple = new Apple();
	std::cout << "----------------------------------" << std::endl;
}
void TestUtf()
{
	GltfReader reader;
	std::shared_ptr<GLTFModel> gltfModel;
	reader.loadAssets("D:\\GitProject\\FEngine\\Assets\\PbrBox\\BoomBox.gltf", *gltfModel.get());
}

ostream& operator<<(ostream& out, Mat4& mat)
{
	out << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << endl;
	out << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << endl;
	out << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << endl;
	out << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << endl;
	return out;
}

ostream& operator<<(ostream& out, Quatf& q)
{
	out << q[0] << " " << q[1] << " " << q[2] << " " << q[3] << endl;

	return out;
}

void coutVec(DirectX::XMVECTOR& v)
{

	std::cout<<"   " << v.m128_f32[0] << " " << v.m128_f32[1] << " " << v.m128_f32[2] << " " << v.m128_f32[3] << endl;
}
void coutMat(DirectX::XMMATRIX& matd)
{
	std::cout<<"------------mm---------------" << endl;
	coutVec(matd.r[0]);
	coutVec(matd.r[1]);
	coutVec(matd.r[2]);
	coutVec(matd.r[3]);
}
void coutfloat3(DirectX::XMFLOAT3& f)
{
	std::cout << "------------ff---------------" << endl;
	std::cout << "   " << f.x << " " << f.y << " " << f.z << " "<< endl;
}
void TestTransform()
{
	//Transform t;
	//XMFLOAT3 m_Position = XMFLOAT3(-4.0f, 4.0f, 0.0f);
	//XMFLOAT3 target = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//t.SetPosition(XMFLOAT3(-4.0f, 4.0f, 0.0f));
	//t.LookAt(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//DirectX::XMMATRIX mat = t.GetLocalToWorldMatrixXM();
	//DirectX::XMMATRIX view = t.GetWorldToLocalMatrixXM();
	//DirectX::XMMATRIX view2 = t.GetWorldToLocalMatrixXM2();
	//XMMATRIX View = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&target), XMLoadFloat3(&up));
	////XMVECTOR R0 = XMVector3Cross(UpDirection, R2);
	//XMFLOAT3 eular = t.GetRotation();
	//XMFLOAT3 f = t.GetForwardAxis();
	//XMFLOAT3 u = t.GetUpAxis();
	//XMFLOAT3 s = t.GetRightAxis();
	//coutfloat3(eular);
	//coutMat(mat);
	//coutMat(view);
	//coutMat(View);
}


void TestMat()
{
	Vec3f center(0.0f, 0.0f, 0.0f);
	Vec3f eye(-4.0f, 4.0f, 0.0f);
	Vec3f up(0.0f, 1.0f, 0.0f);
	Mat4 view(Mat4::lookAt(up, eye, center));
	Quatf _rotation = inverse(view.GetRotate());
	Mat4 world = rotate(_rotation) * translate(eye);
	Mat4 view2 = translate(-eye) * rotate(inverse(_rotation));

	cross(eye, up);
	cout << "world " << endl << world << endl;
	cout << "view " << endl << view << endl;
	cout << "view2 " << endl << view2 << endl;
}





void TestQuat()
{

	Vec3f center(0.0f, 0.0f, 0.0f);
	Vec3f eye(4.0f, 3.0f, 5.0f);
	Vec3f up(0.0f, 1.0f, 0.0f);
	//Vec3f forward = normalize(center - eye);
	//Vec3f up_normal = normalize(up);
	//Vec3f side = normalize(cross(forward, up_normal));
	////Vec3f u = normalize(cross(side, forward));

	//Mat4 cc = translate(-1.0f, 0.0f, 0.0f) * translate(1.0f, 0.0f, 0.0f);
	//Mat4 viewMat = Mat4::lookAt(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(-4.0f, 0.0f, 0.0f), Vec3f(0.0, 0.0, 0.0));
	//Quatf cameraRotate = inverse(viewMat.GetRotate());
	//Vec3f cameraPos = Vec3f(-4.0f, 0.0f, 0.0f);
	//Mat4 cameraModelMatrix = rotate(cameraRotate) * translate(Vec3f(-4.0f, 0.0f, 0.0f));
	//Mat4 cameraViewMatrix = translate(Vec3f(4.0f, 0.0f, 0.0f)) * rotate(inverse(cameraRotate));
	//Mat4 identity = cameraModelMatrix * viewMat;
	//Mat4 identity2 = translate(Vec3f(4.0f, 0.0f, 0.0f)) * translate(Vec3f(-4.0f, 0.0f, 0.0f));
	//cout << viewMat << endl;
	//cout << cameraModelMatrix << endl;
	//cout <<"cameraViewMatrix "<<endl << cameraViewMatrix << endl;
	//cout << identity << endl;
}

int main()
{
	TestTransform();
	TestMat();


	
    //Ptr<UniformDefault> light = new UniformDefault("light");
    ////UniformBasePtr light = new UniformDefault("light", "struct");
    //UniformBasePtr pos = new UniformVec3f("pos", Vec3f(1.0, 0.0, 0.0));
    //UniformBasePtr distance = new UniformDouble("distance",10.0);
    //light->AddSubUniform(pos);
    //light->AddSubUniform(distance);
    //light->Compile();
    //light->Print();

    //std::cout << "111";
	return 0;
}