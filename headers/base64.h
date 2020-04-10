#ifndef BASE64_H
#define BASE64_H

#include <string>

namespace SMTP
{

class Base64
{
    public:
        static const std::string Base64_Encode(const std::string &plainText);
        static const std::string Base64_Decode(const std::string &encodedText);

        static const std::string Base64Url_Encode(const std::string &plainText);
        static const std::string Base64Url_Decode(const std::string &encodedStr); 
};

}

#endif