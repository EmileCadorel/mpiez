#pragma once


#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <mpiez/mpiexception.hh>

namespace mpiez {
    namespace ezsend {

	template<typename T>
	int send(int & proc, int & tag, std::list<T> & value, MPI_Comm comm) {
	    std::vector<T> aux;
	    aux.resize(value.size());
	    int i = 0;
	    for(auto &it : value) {
		aux[i] = it;
		i++;
	    }
	    return MPI_Send(aux.data(), value.size() * sizeof(T), MPI_BYTE, proc, tag, comm);
	}

    };

    namespace ezrecv {

	template <typename T>
	int recv(int & proc, int & tag, std::list<T> & value, MPI_Status & status, MPI_Comm comm) {
	    int size;
	    MPI_Probe(proc, tag, comm, &status);
	    MPI_Get_count(&status, MPI_BYTE, &size);
	    std::vector<T> aux; aux.resize(size / sizeof(T));
	    int i = MPI_Recv(aux.data(), size, MPI_BYTE, proc, tag, comm, MPI_STATUS_IGNORE);
	    value.resize(0);
	    for(auto &it : aux) {
		value.push_back(it);
	    }
	    return i;
	}
	
	
    };
};

