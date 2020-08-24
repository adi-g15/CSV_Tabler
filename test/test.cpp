#include <iostream>
#include <fstream>
#include "../includes/collection.h"
#include "../includes/util.hpp"
#include <vector>
#include <algorithm>

using namespace std;

auto split(const std::string &s, char delim) -> std::vector<std::string>{

	std::vector<std::string> retVec;

	auto start = s.begin(), end = std::find(s.begin(),s.end(), delim);
	clog<<"Found delim \""<<delim<<"\" at "<<end - s.begin()<<endl;
	while( end != s.end() ){
		clog<<"Inside while again from "<<start	- s.begin()<<' '<<end - s.begin()<<endl;

		//this while loops while there is an unclosed quote present
		clog<<"[Before while] -> \""<<s.substr(start - s.begin(), end - start)<<'"'<<endl;
		clog<<"End currently is "<<end-s.begin()<<" and count should be "<<std::count(start, end+1, '"')<<endl;
		while( std::count(start, end, '"')%2 != 0 ){	//ie. odd number of quotes in substring
			if(end == s.end()){
				throw std::logic_error("Parsing Error: Odd number of quotes in passed string, end of input");
			}
				//++end so that it includes the last found returned by std::find
			end = ++std::find(++end, s.end(), '"');
			clog<<"End currently is "<<end-s.begin()<<" and count should be "<<std::count(start, end+1, '"')<<endl;
		}
		clog<<"Got this string -> \""<<s.substr(start - s.begin(), end -  start)<<'"'<<endl;
		retVec.push_back( s.substr(start - s.begin(), end -  start) );

		//incremented just to ignore the delim
		start = end;
		if(end != s.end()){
			start = ++end;
		}
		end = std::find( start , s.end(), delim);
	}
	if(start != s.end()){
		clog<<start - s.begin()<<endl;
		retVec.push_back( s.substr(start - s.begin()) );
	}

	return retVec;
}

void trim(std::string& s){
	auto i = s.end() - 1;
	for ( ; i != s.begin() ; --i)
	{
		if(!isspace(static_cast<unsigned char>(*i))){
			++i;
			break;
		}
	}
	clog<<"From "<<i-s.begin()<<endl;
	s.erase(i, s.end());

	i = s.begin();
	for ( ; i != s.end(); ++i)
	{
		if(!isspace(static_cast<unsigned char>(*i))){
			break;
		}
	}
	s.erase(s.begin(), i);
}

int main(int argc, char const *argv[])
{
	ios::sync_with_stdio(false);

	string s = "  Hi Aditya  ";
	trim(s);
	cout<<'"'<<s<<'"'<<endl;

	// try{
		// string s = "\"Aditya Gupta\". I am \"here\", trying my\" function\" ";
		// cout<<s.length()
		// 	<<endl
		// 	<<s
		// 	<<endl
		// 	<<std::count(9+s.begin(), 22+s.begin(), '"')
		// 	<<endl;
		// auto v = split(s, ' ');
		// cout<<v.size()<<endl;
		// for(auto &&i : v){
		// 	cout<<i<<endl;
		// }
	// }catch(std::logic_error &e){
	// 	cerr<<e.what()<<endl;
	// }

	auto testColl = Collection("Test");
	testColl.parseCSV("test.csv");
	return 0;
}
