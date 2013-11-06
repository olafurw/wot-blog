#ifndef _WOT_TEMPLATES_
#define _WOT_TEMPLATES_

#include <string>
#include <map>

namespace wot
{
namespace templates
{
    std::string parse(const std::string& filename, const std::map<std::string, std::string>& value_map);
}
}

#endif
