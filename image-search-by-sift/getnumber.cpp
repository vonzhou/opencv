#include <iostream>
#include <string>
#include <iomanip> // setfill, setw
#include <sstream> // ostringstream

using namespace std;

int main(){

	std::ostringstream strbuf;
	string buf;

	for(int i=0; i<50;i++){
		strbuf << setfill('0') << setw(4) << i;
		buf = strbuf.str();
		buf = buf + "==";
		cout << buf << endl;

		strbuf.clear();
		strbuf.str("");
		buf = "";
	}
		
}