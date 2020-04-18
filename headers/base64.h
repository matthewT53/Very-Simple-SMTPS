#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <cstdint>

namespace smtp
{

class Base64
{
    public:
        static const std::string Base64Encode(const std::vector<uint8_t> &data);
        static const std::vector<uint8_t> Base64Decode(const std::string &data);

        static const std::string Base64UrlEncode(const std::vector<uint8_t> &data, bool keep_padding = false);
        static const std::vector<uint8_t> Base64UrlDecode(const std::string &data); 
};

}

#endif