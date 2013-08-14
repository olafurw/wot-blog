#include <iostream>
#include <vector>

#include "soci/soci.h"
#include "soci/mysql/soci-mysql.h"

#include "../utils.hpp"

struct blog_entry
{
    unsigned int id;
    std::string title;
    std::string body;
    unsigned int created;
    unsigned int up;
    unsigned int down;

    blog_entry()
    {
        id = 0;
        title = "";
        body = "";
        created = 0;
        up = 0;
        down = 0;
    }
};

namespace soci
{
template<> struct type_conversion<blog_entry>
{
    typedef values base_type;
    static void from_base(values const & v, indicator /* ind */, blog_entry& be)
    {
        be.id = v.get<int>("id");
        be.title = v.get<std::string>("title");
        be.body = v.get<std::string>("body");
        be.created = v.get<int>("created");
        be.up = v.get<int>("up");
        be.down = v.get<int>("down");
    }
    static void to_base(const blog_entry& be, values & v, indicator & ind)
    {
        v.set("title", be.title);
        v.set("body", be.body);
        ind = i_ok;
    }
};
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        return 0;
    }

    try
    {
        // Must start with /blog/ and something after that
        std::string argument_str(argv[1]);
        if(argument_str.find("/blog/") != 0 && argument_str.size() <= 6)
        {
            return 0;
        }

        // Must be split into at least 3 parts
        std::vector<std::string> url_head = wot::utils::split_string(argument_str, '/');
        if(url_head.size() < 3)
        {
            return 0;
        }

        int blog_id = 0;

        try
        {
            blog_id = std::stoi(url_head.at(2));
        }
        catch(std::invalid_argument& ex)
        {
            return 0;
        }
        catch(std::out_of_range& ex)
        {
            return 0;
        }

        std::string database_info = wot::utils::file_to_string("database.info");
        std::vector<std::string> database_lines = wot::utils::split_string(database_info, '\n');

        std::string connection_str = "host=" + database_lines.at(0) + " db=" + database_lines.at(1) + " user=" + database_lines.at(2);

        std::shared_ptr<soci::session> sql = std::make_shared<soci::session>(soci::mysql, connection_str);

        blog_entry be;
        *sql << "SELECT id, title, body, created, up, down FROM blog WHERE id = :id", soci::use(blog_id), soci::into(be);

        std::cout << "<h3>" << be.title << "</h3><br />";
        std::cout << be.body;
    }
    catch(...)
    {
        std::cout << "oops" << std::endl;
    }
    return 0;
}
