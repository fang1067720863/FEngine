
#include<vector>
#include<iostream>
#include"ShaderParameter.h"

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