#pragma once

class ComparableLibirary
{
    template<typename Type>
    static int compare (const Type& a,const Type& b);
};

template <typename Type>
int ComparableLibirary::compare(const Type& a, const Type& b)
{
    if(a > b)
        return 1;
    else if (a<b)
        return -1;
    return 0;
}
