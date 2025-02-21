#pragma once


class Member_Init_Order_Matters
{
public:

	Member_Init_Order_Matters() = default;

	//The crazy thing here is that the member initializer below initializes the variables in the order they're declared in the class
	Member_Init_Order_Matters(int first, int second) : firstNum(first), secondNum(firstNum + 1) {};
	//So, since secondNum is declared below, first, it will get value in firstNum (which is garbage) before firstNum is initialized.
	//I have initialized firstNum to technically be 0 in the class here but secondNum would STILL not have the "expected" value.

	int GetSecondNum() { return secondNum; }

private:

	int secondNum = 0;
	int firstNum = 0;

};

