#include <stdafx.hpp>
#include <Point3D.hpp>
#include <BaseModel.hpp>
#include <RichModel.hpp>
#include <ExactMethodForDGP.hpp>
#include <PreviousCH.hpp>
#include <ImprovedCHWithEdgeValve.hpp>
#include <ICHWithFurtherPriorityQueue.hpp>
/**
contents of X.hpp
-----------------
struct X
{
	double x, y, z;
	X();
	X(double x, double y, double z);
};


contents of A.hpp
-----------------
#include <vector>
#include <X.hpp>

class A
{
	public std::vector<X> m_contents;
	A();
	void addContents(std::vector<X> contents);
};

void A::addContents(std::vector<X> contents)
{
	for (X item : contents)
	{
		m_contents.push_back(X(item.x, item.y, item.z))
	}
}

contents of B.hpp
-----------------
class B: virtual public A
{
	B();
};


in Python (version 3.5)
-----------------

>>>from hello import X, A, B;
>>>from random import random;
>>>contents = [X(random(), random(), random()) for i in range(20)];
>>>aa = A();
>>>bb = B();
>>>aa.addContents();
>>>bb.addContents();

12139 Segmentation fault      (core dumped) python3

**/
