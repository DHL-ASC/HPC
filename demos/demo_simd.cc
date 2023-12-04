#include <iostream>
#include <sstream>


#include <simd.h>

using namespace DHL_HPC;
using std::cout, std::endl;

auto Func1 (SIMD<double> a, SIMD<double> b)
{
  return a+b;
}

auto Func2 (SIMD<double,4> a, SIMD<double,4> b)
{
  return a+3*b;
}

auto Func3 (SIMD<double,4> a, SIMD<double,4> b, SIMD<double,4> c)
{
  return FMA(a,b,c);
}


auto Load (double * p)
{
  return SIMD<double,2>(p);
}

auto LoadMask (double * p, SIMD<mask64,2> m)
{
  return SIMD<double,2>(p, m);
}

auto TestSelect (SIMD<mask64,2> m, SIMD<double,2> a, SIMD<double,2> b)
{
  return Select (m, a, b);
}

SIMD<double,2> TestHSum (SIMD<double,4> a, SIMD<double,4> b)
{
  return HSum(a,b);
}



int main()
{
  SIMD<double,4> a(1.,2.,3.,4.);
  SIMD<double,4> b(1.0);
  
  cout << "a = " << a << endl;
  cout << "b = " << b << endl;
  cout << "a+b = " << a+b << endl;

  cout << "HSum(a) = " << HSum(a) << endl;
  cout << "HSum(a,b) = " << HSum(a,b) << endl;

  
  auto sequ = IndexSequence<int64_t, 4>();
  cout << "sequ = " << sequ << endl;
  auto mask = (2 >= sequ);
  cout << "2 >= " << sequ << " = " << mask << endl;

  {
    double a[] = { 10, 10, 10, 10 };
    SIMD<double,4> sa(&a[0], mask);
    cout << "sa = " << sa << endl;
  }

  cout << "Select(mask, a, b) = " << Select(mask, a,b) << endl;
  
  {
    // ARM block
    #if defined(__aarch64__) || defined(_M_ARM64)
    SIMD<double,2> c(4.,5.);
    SIMD<double,2> d(4.,2.);
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;

    auto mask2 = (d >= c);
    auto equal = (c == d);

    cout << "c * d = " << c*d << endl;
    cout << "c / d = " << c/d << endl;
    cout << "10 / d = " << 10/d << endl;
    cout << "d >= c = " << mask2 << endl;
    cout << "c == d = " << equal << endl;
    #endif
  }
}
