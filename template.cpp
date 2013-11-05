#include <iostream>
#include <sstream>
#include <map>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "../utils.hpp"

void parse_template_segment(const std::string& segment, const std::map<std::string, std::string>& template_data)
{
    if(segment[0] != '{' || segment[segment.size() - 1] != '}')
    {
        std::cout << "error parsing segment" << std::endl;
        return;
    }

    std::cout << segment << std::endl;
}

int main()
{
    boost::regex exp("\\{([^{]+)\\}", boost::regex::extended);
    std::stringstream ss;

    std::map<std::string, std::string> template_data;
    template_data["bleh"] = "Blegh";
    template_data["bleh2"] = "Blegh 2";
    template_data["dableh"] = "Da Blegh";
    template_data["something else"] = "Something totally different\nand\ninteresting stuff!";

    std::vector<std::string> file_data = wot::utils::file_to_array("body.tpl");

    for(std::string line : file_data)
    {
        auto flags = boost::match_default;
        boost::match_results<std::string::const_iterator> matches;
        auto startPos = line.cbegin();
        auto endPos = line.cend();

        while(boost::regex_search(startPos, endPos, matches, exp, flags))
        {
            std::string segment = std::string(matches[0]);
            std::string content = std::string(matches[1]);

            boost::replace_all(line, segment, template_data[content]);

            // line has changed, reset the start and end pos to search again
            startPos = line.cbegin();
            endPos = line.cend();
        }

        ss << line << std::endl;
    }

    std::cout << ss.str() << std::endl;

    return 0;
}
