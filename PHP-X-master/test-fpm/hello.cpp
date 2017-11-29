#include <iostream>
#include <vector>
#include <string>
using namespace std;
class ArgInfo{};

struct MAN{
	int x;
	char y;
};
int main(){
	MAN m;
	cout << m.x << endl;
	return 1;
	vector<char> str(1,0);
	int nLen = sprintf(&str[0],"hello %s world" , "nice");

	string s;
	s.append(&str[0], nLen);
	s.resize(nLen);
	cout << nLen << endl << s << endl;
}
