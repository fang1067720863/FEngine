#include"UtfConverter.h"


// use count to avoid erronous utf8 encodings causing reading reading beyond size of buffer.
template<typename Iterator, class Func>
bool decode_utf8(Iterator itr, size_t count, Func op)
{
    while (count > 0)
    {
        auto c0 = *itr++;
        --count;

        if ((c0 & 0x80) == 0) // 1-byte ascii character
        {
            op(c0);
            continue;
        }

        if (count == 0) return false;

        auto c1 = *itr++;
        --count;
        if ((c0 & 0xe0) == 0xc0) // 2-byte character
        {
            op(((c0 & 0x1f) << 6) | (c1 & 0x3f));
            continue;
        }

        if (count == 0) return false;

        auto c2 = *itr++;
        --count;
        if ((c0 & 0xf0) == 0xe0) // 3-byte character
        {
            op(((c0 & 0xf) << 12) | ((c1 & 0x3f) << 6) | (c2 & 0x3f));
            continue;
        }

        if (count == 0) return false;

        auto c3 = *itr++;
        --count;

        // 4-bute character. Could check for char0 < 0xf8 to ensure encoding is valid?
        op(((c0 & 0x7) << 18) | ((c1 & 0x3f) << 12) | ((c2 & 0x3f) << 6) | (c3 & 0x3f));
    }

    return true;
}

template<typename Iterator, class Func>
bool encode_utf8(Iterator itr, Iterator end, Func op)
{
    while (itr != end)
    {
        uint32_t c = *itr++;
        if (c < 0x80) // 1-byte ascii character
        {
            op(c);
            continue;
        }

        if (c < 0x800) // 2-byte character
        {
            op(0xc0 | ((c >> 6) & 0x1f)); // byte 1
            op(0x80 | (c & 0x3f));        // byte 2
            continue;
        }

        if (c < 0x10000) // 3-byte character
        {
            op(0xe0 | ((c >> 12) & 0x0f)); // byte 1
            op(0x80 | ((c >> 6) & 0x3f));  // byte 2
            op(0x80 | (c & 0x3f));         // byte 3
            continue;
        }

        // 4 byte character
        op(0xf0 | ((c >> 18) & 0x07)); // byte 1
        op(0x80 | ((c >> 12) & 0x3f)); // byte 2
        op(0x80 | ((c >> 6) & 0x3f));  // byte 3
        op(0x80 | (c & 0x3f));         // byte 4
    }

    return true;
}

std::wstring ConvertUtf(const std::string& utf8)
{
    std::wstring dst;
    //todo Return value optimization?
    decode_utf8(utf8.begin(), utf8.size(), [&dst](uint32_t c) { dst.push_back(c); });
    return dst;
}

void ConvertUtf(const std::wstring& src, std::string& utf8)
{
    utf8.clear();
    encode_utf8(src.begin(), src.end(), [&utf8](uint32_t c) { utf8.push_back(static_cast<char>(c)); });
}
bool FileExists(const wchar_t* path)
{
#if defined(WIN32)
    return _waccess(path, 0) == 0;
#else
    return false;
#endif
}



bool FindGlobalPath()
{
    std::wstring shaderFile = ConvertUtf(std::string("Shader\\DefaultVertex.hlsl"));
    if (FileExists(shaderFile.c_str()))
    {
        GLOBAL_PATH = "";
    }
    else {
        GLOBAL_PATH = "D://GitProject//FEngine//FRenderer//";
    }
    GLOBAL_PATH = "D://GitProject//FEngine//FRenderer//";
    return true;
}
