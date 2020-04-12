#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <cstdint>

namespace smtp
{

class Base64
{
    public:
        static const std::string Base64_Encode(const std::vector<uint8_t> &data);
        static const std::vector<uint8_t> Base64_Decode(const std::string &data);

        static const std::string Base64Url_Encode(const std::vector<uint8_t> &data);
        static const std::vector<uint8_t> Base64Url_Decode(const std::string &data); 
};

}

#endif