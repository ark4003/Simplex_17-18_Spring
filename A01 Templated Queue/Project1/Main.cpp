#include "Main.h"
#include "TempQueue.h"


class Testin
{
public:
	int thing1 = 0;

};


int main(void)
{


	std::cout << "testing floats \n";
	tempQueue<float> tq;

	tq.Pop();
	tq.Print();
	tq.Push(1.0f);
	tq.Push(3.0f);
	//tq.TestDelete();
	tq.Print();
	tq.Pop();
	tq.Print();
	tq.Push(5.6f);
	tq.Push(7.0f);
	tq.Push(4.5f);
	tq.Print();
	tq.Pop();
	tq.Print();

	std::cout << "\n testing ints \n";
	tempQueue<int> tqi;
	tqi.Print();
	tqi.Push(1);
	tqi.Push(3);
	tqi.Print();
	tqi.Pop();
	tqi.Print();
	tqi.Push(5);
	tqi.Push(7);
	tqi.Push(4);
	tqi.Print();
	tqi.Pop();
	tqi.Print();

	std::cout << "\n testing chars \n";
	tempQueue<char> tqc;
	tqc.Print();
	tqc.Push('t');
	tqc.Push('e');
	tqc.Print();
	tqc.Pop();
	tqc.Print();
	tqc.Push('s');
	tqc.Push('t');
	tqc.Push('c');
	tqc.Print();
	tqc.Pop();
	tqc.Print();

	std::cout << "Press enter";
	getchar();
	return 0;
}



