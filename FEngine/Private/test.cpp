
#include<vector>
#include<iostream>
#include"ShaderParameter.h"

class Apple:public FReference {

public:
	Apple() { val = "123"; std::cout << " Apple default ctor" << std::endl; }
	Apple(const Apple& a) { val = a.val;  std::cout << " Apple copy ctor" << std::endl; }
	Apple& operator= (const Apple& a) { val = 1; std::cout << "Apple copy assgin" << std::endl; return *this; }
	// a.val �౾�����ƶ����������Լ����ƶ����� ��string���ƶ��ߣ�int û���ƶ�����͵��ÿ������� 
	Apple(Apple&& a) :val(std::move(a.val)) { std::cout << "Apple move ctor" << std::endl; }      // һ���ƶ��Ǽ��������ƶ� ����std::move(a.val)  �� move(int) �����ǿ���
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
	Ptr<Apple> newapple(std::move(apple));
	std::cout << "----------------------------------" << std::endl;
}
int main()
{
	TestPtr();
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