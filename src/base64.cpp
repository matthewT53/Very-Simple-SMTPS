#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <unordered_map>

#include "base64.h"

namespace smtp
{

using byte = uint8_t;

static const std::string kTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const std::string kTableUrl = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static const std::unordered_map<byte, byte> kDecodeTable = {
    {'A', 0x0}, {'B', 0x1}, {'C', 0x2}, {'D', 0x3}, {'E', 0x4}, {'F', 0x5}, {'G', 0x6}, {'H', 0x7}, {'I', 0x8}, {'J', 0x9}, {'K', 0xa}, {'L', 0xb}, 
    {'M', 0xc}, {'N', 0xd}, {'O', 0xe}, {'P', 0xf}, {'Q', 0x10}, {'R', 0x11}, {'S', 0x12}, {'T', 0x13}, {'U', 0x14}, {'V', 0x15}, {'W', 0x16}, 
    {'X', 0x17}, {'Y', 0x18}, {'Z', 0x19}, {'a', 0x1a}, {'b', 0x1b}, {'c', 0x1c}, {'d', 0x1d}, {'e', 0x1e}, {'f', 0x1f}, {'g', 0x20}, {'h', 0x21}, 
    {'i', 0x22}, {'j', 0x23}, {'k', 0x24}, {'l', 0x25}, {'m', 0x26}, {'n', 0x27}, {'o', 0x28}, {'p', 0x29}, {'q', 0x2a}, {'r', 0x2b}, {'s', 0x2c}, 
    {'t', 0x2d}, {'u', 0x2e}, {'v', 0x2f}, {'w', 0x30}, {'x', 0x31}, {'y', 0x32}, {'z', 0x33}, {'0', 0x34}, {'1', 0x35}, {'2', 0x36}, {'3', 0x37}, 
    {'4', 0x38}, {'5', 0x39}, {'6', 0x3a}, {'7', 0x3b}, {'8', 0x3c}, {'9', 0x3d}, {'-', 0x3e}, {'_', 0x3f}, {'+', 0x3e}, {'/', 0x3f}, {'=', 0x0},
};

const std::string DoBase64Encode( const std::vector<byte> &data, const std::string &table );
const std::string DoBase64EncodeBlock( const std::vector<byte> &data_block, const std::string &table, int padding = 0 );

const std::vector<byte> DoBase64Decode( const std::string &data );
const std::vector<byte> DoBase64DecodeBlock( const std::string &data_block, int padding = 0 );

const std::string smtp::Base64::Base64Encode( const std::vector<byte> &data )
{
    return DoBase64Encode( data, kTable );
}

const std::vector<byte> smtp::Base64::Base64Decode(const std::string &data)
{
    return DoBase64Decode( data );
}

const std::string smtp::Base64::Base64UrlEncode(const std::vector<byte> &data, bool keep_padding)
{
    std::string result = DoBase64Encode( data, kTableUrl );
    if ( !keep_padding )
    {
        result.erase(std::remove( result.begin(), result.end(), '='), result.end() );
    }
    return result;
}

const std::vector<byte> smtp::Base64::Base64UrlDecode(const std::string &data)
{
    return DoBase64Decode( data );
}

const std::string DoBase64Encode(const std::vector<byte> &data, const std::string &table)
{
    std::string result;

    int i = 0;
    int n = static_cast<int>(data.size());
    
    std::vector<byte> arr(3, 0x0);

    for (byte b : data)
    {
        arr[i++ % 3] = b;

        if (i % 3 == 0 || i == n)
        {
            result += DoBase64EncodeBlock(arr, table, i % 3);
            std::fill(arr.begin(), arr.end(), 0x0);
        }
    }

    return result;
}

/***
 * Base64 encodes a block of plaintext. 
 * It is assumed that data_block contains a maximum of 3 bytes in size. 
 */
const std::string DoBase64EncodeBlock(const std::vector<byte> &data_block, const std::string &table, int padding)
{
    int i1 = (data_block[0] & 0xfc) >> 2;
    int i2 = ((data_block[0] & 0x3) << 4) | ((data_block[1] & 0xf0) >> 4);
    int i3 = ((data_block[1] & 0xf) << 2) | ((data_block[2] & 0xc0) >> 6);
    int i4 = (data_block[2] & 0x3f);

    std::string s(4, 0x00);
    s[0] = table[i1]; 
    s[1] = table[i2];
    s[2] = (padding == 1) ? '=' : table[i3];
    s[3] = (padding >= 1) ? '=' : table[i4];
    return s;
}

const std::vector<byte> DoBase64Decode(const std::string &data)
{
    std::vector<byte> result;

    int i = 0;
    int n = static_cast<int>( data.length() );
    std::string arr(4, 0x0);

    for (byte b : data)
    {
        arr[i++ % 4] = b;

        if (i % 4 == 0 || i == n)
        {
            const std::vector<byte> decoded = DoBase64DecodeBlock(arr, i % 4);
            result.insert(result.end(), decoded.begin(), decoded.end());
        }
    }

    return result;
}

/***
 * This function base64 decodes a block of 4 base64 encoded characters.
 * It is assumed that data_block is 4 bytes in size.
 */
const std::vector<byte> DoBase64DecodeBlock(const std::string &data_block, int num_chars)
{
    std::vector<byte> result(3);

    byte i1 = kDecodeTable.find(data_block[0])->second;
    byte i2 = kDecodeTable.find(data_block[1])->second;
    byte i3 = kDecodeTable.find(data_block[2])->second;
    byte i4 = kDecodeTable.find(data_block[3])->second;

    result[0] = ((i1 & 0x3f) << 2) | ((i2 & 0x30) >> 4);
    result[1] = ((i2 & 0xf) << 4) | ((i3 & 0x3c) >> 2);
    result[2] = ((i3 & 0x3) << 6) | i4;

    size_t padding_pos = data_block.find_first_of("=");
    int n_chars = (padding_pos == std::string::npos) ? num_chars : static_cast<int>( padding_pos );

    // Padding will either be 2 or 3, since it is not possible to have 1 byte
    if (n_chars > 0)
    {
        int num_remove = 4 - n_chars;
        result.resize(3 - num_remove);
    }

    return result;
}

}
