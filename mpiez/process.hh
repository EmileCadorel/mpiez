#pragma once

#include <mpiez/protocol.hh>

namespace mpiez {

    template <typename T>
    struct process {
	process(T & proto) : m_proto(proto) {
	}

	virtual void routine() = 0;	
	
	virtual void on_end() = 0;
	
	T & m_proto;
    };

    

};
