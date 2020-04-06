#include "uan-class.h"

bool State::operator < (const State &s) const
{
    if(id < s.id || (id == s.id && connect < s.connect) || (id == s.id && connect == s.connect && bandwidth < s.bandwidth) ){
        return true;
    }
    else{
        return false;
    }
}