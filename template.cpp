#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../utils.hpp"

namespace wot
{
namespace templates
{
    void parse_template_segment(const std::string& segment, const std::map<std::string, std::string>& template_data)
    {
        if(segment[0] != '{' || segment[segment.size() - 1] != '}')
        {
            std::cout << "error parsing segment" << std::endl;
            return;
        }

        std::cout << segment << std::endl;
    }

    std::string parse(const std::string& filename, const std::map<std::string, std::string>& value_map)
    {
        static boost::regex exp("\\{([^{]+)\\}", boost::regex::extended);
        std::stringstream ss;

        auto file_data = wot::utils::file_to_array(filename.c_str());

        for(auto& line : file_data)
        {
            auto startPos = line.cbegin();
            auto endPos = line.cend();
            
            auto flags = boost::match_default;
            boost::match_results<std::string::const_iterator> matches;

            while(boost::regex_search(startPos, endPos, matches, exp, flags))
            {
                std::string segment = std::string(matches[0]);
                std::string content = std::string(matches[1]);

                boost::replace_all(line, segment, value_map.at(content));

                // line has changed, reset the start and end pos to search again
                startPos = line.cbegin();
                endPos = line.cend();
            }

            ss << line << std::endl;
        }

        return ss.str();
    }
}
}
