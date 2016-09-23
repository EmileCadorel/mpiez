#pragma once

#include <mpiez/message.hh>
#include <mpiez/smessage.hh>

namespace mpiez {

    struct protocol {

	protocol(int id, int total) : id(id), total(total) {}
	
	int id;
	int total;
	
    };

};
