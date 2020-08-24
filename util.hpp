#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>	//For std::logic_error

namespace util{
	void trim(std::string& s);
	auto split(const std::string& s, char delim = ',') -> std::vector<std::string>;
}

void util::trim(std::string& s){
	auto i = s.end() - 1;
	for ( ; i != s.begin() ; --i)
	{
		if(!isspace(static_cast<unsigned char>(*i))){
			++i;
			break;
		}
	}
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

auto util::split(const std::string &s, char delim) -> std::vector<std::string>{

	std::vector<std::string> retVec;

	auto start = s.begin(), end = std::find(s.begin(),s.end(), delim);
	while( end != s.end() ){

		//this while loops while there is an unclosed quote present
		while( std::count(start, end, '"')%2 != 0 ){	//ie. odd number of quotes
			if(end == s.end()){
				throw std::logic_error("Parsing Error: Odd number of quotes in passed string, end of input");
			}
				//++end so that it includes the last found returned by std::find
			end = ++std::find(++end, s.end(), '"');
		}
		retVec.push_back( s.substr(start - s.begin(), end -  start) );

		//incremented just to ignore the delim
		start = end;
		if(end != s.end()){
			start = ++end;
		}
		end = std::find( start , s.end(), delim);
	}
	if(start != s.end()){
		retVec.push_back( s.substr(start - s.begin()) );
	}

	return retVec;
}