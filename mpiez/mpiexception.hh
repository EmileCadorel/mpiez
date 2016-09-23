#pragma once


#include <exception>
#include <string>

namespace mpiez {
    
    struct mpiezexception {
	mpiezexception (const char * t) : text(t) {}
	std::string text;
    };

    struct out_of_index : mpiezexception {
	out_of_index(int i) : mpiezexception((std::string("out of index : ") + std::to_string(i)).c_str()) {
	}
    };
    
    
};
