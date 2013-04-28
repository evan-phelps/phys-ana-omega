#ifndef CONFIGDATA_H_
#define CONFIGDATA_H_
#include <iostream>
#include <map>
#include <string>
namespace ConfigData
{
struct data : std::map <std::string, std::string> {
	bool iskey( const std::string& s ) const {
		return count( s ) != 0;
	}
};
std::istream& operator >> ( std::istream& ins, data& d );
std::ostream& operator << ( std::ostream& outs, const data& d );
}
#endif /* CONFIGDATA_H_ */
