class Hello
{
public:
  Hello();
  ~Hello();
  void classFunc();
};

namespace test {
  namespace base {
    extern void nspFunc(void);
    //void nspFunc(void) is wrong, must add extern;
  }
}
