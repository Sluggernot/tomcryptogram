#pragma once

class Function_Arg_Order
{
public: 

	static int ReceiveSomeInputAndReturnCalc(int a, int b, int c)
	{
		return a + b + c;
	}

	static int Calc(int in) { return ++in; }
};

