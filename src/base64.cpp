#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>

#include "base64.h"

namespace smtp
{

using byte = uint8_t;

static const std::string kTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const std::string kTableUrl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

const std::string DoBase64Encode(const std::vector<byte> &data, const std::string &table);
const std::string DoBase64EncodeBlock(byte arr[], const std::string &table, int padding = 0);

const std::string smtp::Base64::Base64_Encode(const std::vector<byte> &data)
{
    return DoBase64Encode(data, kTable);
}

const std::string smtp::Base64::Base64_Decode(const std::vector<byte> &data)
{
    //TODO
    std::string result;
    return result;
}

const std::string smtp::Base64::Base64Url_Encode(const std::vector<byte> &data)
{
    std::string result = DoBase64Encode(data, kTableUrl);
    result.erase(std::remove(result.begin(), result.end(), '='), result.end());
    return result;
}

const std::string smtp::Base64::Base64Url_Decode(const std::vector<byte> &data)
{
    //TODO
    std::string result;
    return result;
}

const std::string DoBase64Encode(const std::vector<byte> &data, const std::string &table)
{
    std::string result;

    int i = 0;
    int n = static_cast<int>(data.size());
    byte arr[3] = {0};
    for (byte b : data)
    {
        arr[i % 3] = b;
        i++;

        if (i % 3 == 0 || i == n)
        {
            result += DoBase64EncodeBlock(arr, table, i % 3);
        }
    }

    return result;
}

const std::string DoBase64EncodeBlock(byte arr[], const std::string &table, int padding)
{
    int i1 = (arr[0] & 0xfc) >> 2;
    int i2 = ((arr[0] & 0x3) << 4) | ((arr[1] & 0xf0) >> 4);
    int i3 = ((arr[1] & 0xf) << 2) | ((arr[2] & 0xc0) >> 6);
    int i4 = (arr[2] & 0x3f);

    std::string s(4, 0x00);
    s[0] = table[i1]; 
    s[1] = table[i2];
    s[2] = (padding == 1) ? '=' : table[i3];
    s[3] = (padding >= 1) ? '=' : table[i4];

    std::fill_n(arr, 3, 0);
    return s;
}

}
