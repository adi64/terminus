#pragma once

#include <string>

namespace terminus
{

class Material
{
public:
    static Material * loadMTX(std::string path);
public:
    Material();
};

}//namespace terminus
