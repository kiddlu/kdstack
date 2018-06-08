class MyClass
{
public:
	int add(int a, int b)
	{
		return (a + b);
	}
};

extern "C" int add_cpp(int a, int b);

int add_cpp(int a, int b)
{
	MyClass CppClass;
	return CppClass.add(a, b);      
}
