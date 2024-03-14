#include <iostream>
#include <iterator>
#include "ChunkList.h"
#include <list>

using namespace std;

void print(ChunkList<int, 9> list) {
	for (auto e : list) {
		cout << e << " ";
	}
	cout << "\n";
}

int main()
{

	list<int> l = { 1, 2, 3 };

	ChunkList<int, 9> l1(l.begin(), l.end());

	list<int> ll = { 1, 2, 3, 0, 0, 0 };

	ChunkList<int, 9> l2(ll.begin(), ll.end());

	l1.resize(6);

	print(l1);

	bool ans = l1 == l2;

	l1.resize(1);
	l2.resize(1);

	bool ans1 = l1 == l2;

	cout << ans << " " << ans1;

	return 0;
}