#include "reader.h"

#include <string>
#include <iostream>

#include "types/color.h"
#include "types/table.h"
#include "types/object.h"

#include "util/demangle.h"

Reader::Reader(std::ifstream &file)
{
    this->file = &file;
}

std::any Reader::parse()
{
    int type = file->get();
    // std::cout << "type: " << (char) type << "\n";

    if (file->eof()) std::__throw_ios_failure("Unexpected EOF");

    auto ios_failure = [](std::string err) {
        std::__throw_ios_failure(err.c_str());
    };

    auto read_fixnum = [this, ios_failure]() -> i32 {
        u32 x;
        i8  c = file->get();

        if (c == 0) return 0;
        if (c > 0)
        {
            if (4 < c && c < 128) return c - 5;
            if (c > sizeof(i32)) ios_failure("Fixnum too big: " + std::to_string(c));

            x = 0;
            for (int i = 0; i < 4; i++) x = (i < c ? file->get() << 24 : 0) | (x >> 8);
        }
        else
        {
            if (-129 < c && c < -4) return c + 5;
            c = -c;
            if (c > sizeof(i32)) ios_failure("Fixnum too big: " + std::to_string(c));

            x              = -1;
            const u32 mask = ~(0xff << 24);
            for (int i = 0; i < 4; i++) x = (i < c ? file->get() << 24 : 0) | ((x >> 8) & mask);
        }

        return x;
    };

    switch (type)
    {
    case '@':    // Link
    {
        i32 index = read_fixnum();
        return object_cache.at(index - 1);
    }
    break;
    case 'I':    // IVar
        ios_failure("Not yet Implemented: IVar");
    case 'e':    // Extended
        ios_failure("Not yet Implemented: Extended");
    case 'C':    // UClass
        ios_failure("Not yet Implemented: UClass");
    case '0':    // Nil
        return NULL;
    case 'T':    // True
        return true;
    case 'F':    // False
        return false;
    case 'i':    // Fixnum
        return read_fixnum();
    case 'f':    // Float
    {
        auto str = std::string(read_fixnum(), '0');
        file->read(str.data(), str.length());

        double v;
        if (str.compare("nan") == 0) v = std::nan("");
        if (str.compare("inf") == 0) v = DOUBLE_INF;
        if (str.compare("-inf") == 0)
            v = DOUBLE_NINF;
        else
            v = ::strtod(str.c_str(), NULL);

        object_cache.push_back(v);
        return v;
    }
    break;
    case 'l':    // Bignum
    {
        int  sign = file->get();
        auto len  = read_fixnum();

        char data[len * 2];
        file->read(data, len * 2);

        // TODO

        ios_failure("Not yet Implemented: Bignum");
        return NULL;
    }
    break;
    case '"':    // String
    {
        auto str = std::string(read_fixnum(), '0');
        file->read(str.data(), str.length());

        object_cache.push_back(str);
        return str;
    }
    case '/':    // RegExp
        ios_failure("Not yet Implemented: RegExp");
        break;
    case '[':    // Array
    {
        std::vector<std::any> arr;
        i32                   len = read_fixnum();

        arr.reserve(len);
        for (long i = 0; i < len; i++) arr.push_back(parse());

        object_cache.push_back(arr);
        return arr;
    }
    case '{':    // Hash
    {
        std::unordered_map<i32, std::any> map;
        auto                              length = read_fixnum();

        for (i32 i = 0; i < length; i++)
        {
            auto key = parse();
            if (key.type() != typeid(i32))
                ios_failure(std::string("Hash key not Fixnum: ") + ::demangle(key.type().name()));

            auto value = parse();

            map.insert(std::pair(std::any_cast<i32>(key), value));
        }

        object_cache.push_back(map);
        return map;
    }
    break;
    case '}':    // HashDef
        ios_failure("Not yet Implemented: HashDef");
        break;
    case 'S':    // Struct
        ios_failure("Not yet Implemented: Struct");
        break;
    case 'u':    // UserDef
    {
        std::string name;
        auto        pname = parse();
        if (pname.type() == typeid(std::vector<u8>))
        {
            auto vec = std::any_cast<std::vector<u8>>(pname);
            name.reserve(vec.size());
            name.append(vec.begin(), vec.end());
        }
        else
            ios_failure(std::string("UserDef name not Symbol"));

        auto size = read_fixnum();

        if (name.compare("Color") == 0)
        {
            Color color;
            file->read((char *) &color.red, sizeof(double));
            file->read((char *) &color.green, sizeof(double));
            file->read((char *) &color.blue, sizeof(double));
            file->read((char *) &color.alpha, sizeof(double));

            object_cache.push_back(color);
            return color;
        }
        else if (name.compare("Table") == 0)
        {
            Table table;

            file->seekg(4, std::ios::cur);
            file->read((char *) &table.x_size, 4);
            file->read((char *) &table.y_size, 4);
            file->read((char *) &table.z_size, 4);

            file->read((char *) &table.total_size, sizeof(i32));
            table.data = std::make_shared<i16[]>(table.total_size);

            for (i32 i = 0; i < table.total_size; i++)
                file->read((char *) &table.data[i], sizeof(i16));

            object_cache.push_back(std::make_any<Table>(table));
            return object_cache.back();
        }
        else
            ios_failure("Unsupported user defined class: " + name);
    }
    break;
    case 'U':    // User Marshal
        ios_failure("Not yet Implemented: User Marshal");
        break;
    case 'o':    // Object
    {
        Object o;
        auto   name = parse();
        if (name.type() == typeid(std::vector<u8>))
        {
            auto vec = std::any_cast<std::vector<u8>>(name);
            o.name.reserve(vec.size());
            o.name.append(vec.begin(), vec.end());
        }
        else
            ios_failure(std::string("Object name not Symbol"));

        auto length = read_fixnum();

        for (i32 i = 0; i < length; i++)
        {
            std::string key;
            {
                auto akey = parse();
                if (akey.type() != typeid(std::vector<u8>))
                    ios_failure(
                      std::string("Object key not symbol: ") + ::demangle(akey.type().name()));

                auto vec = std::any_cast<std::vector<u8>>(akey);
                key      = std::string(vec.begin(), vec.end());
            }

            if (key[0] != '@') ios_failure("Object Key not instance variable name");

            auto value = parse();

            o.list.insert(std::pair(key, value));
        }

        object_cache.push_back(o);
        return o;
    }
    break;
    case 'd':    // Data
        ios_failure("Not yet Implemented: Data");
        break;
    case 'M':    // Module Old
    case 'c':    // Class
    case 'm':    // Module
        ios_failure("Not yet Implemented: Module/Class");
    case ':':    // Symbol
    {
        std::vector<u8> arr;
        i32             len = read_fixnum();

        arr.resize(len);
        file->read((char *) arr.data(), len);

        symbol_cache.push_back(arr);
        return arr;
    }
    break;
    case ';':    // Symlink
    {
        i32 index = read_fixnum();
        return symbol_cache.at(index);
    }
    break;
    default: ios_failure("Unknown Value: " + std::to_string(type));
    }
    return NULL;
}