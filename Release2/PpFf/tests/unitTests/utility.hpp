
#include <sstream>

template <typename T>
std::string ConvertNumberToString ( T Number )
{
 std::ostringstream ss;
 ss << Number;
 return ss.str();
}
