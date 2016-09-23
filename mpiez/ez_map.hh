#pragma once

#include <mpiez/mpiexception.hh>
#include <mpiez/stream.hh>
#include <map>

namespace mpiez {

    namespace ez_send {

	template <typename K, typename V>
	int send(int & proc, int & tag, std::map<K, V> & value, MPI_Comm comm) {
	    std::vector<std::pair<K, V>> aux;
	    aux.resize(value.size());
	    int i = 0;
	    for(auto &it : value) {
		aux[i] = it;
		i++;
	    }
	    return MPI_Send(aux.data(), value.size() * sizeof(std::pair<K, V>), MPI_BYTE, proc, tag, comm);
	}
	
    
    };

    namespace ezrecv {

	template <typename K, typename V>
	int recv(int & proc, int & tag, std::map<K, V> & value, MPI_Status & status, MPI_Comm comm) {
	    int size;
	    MPI_Probe(proc, tag, comm, &status);
	    MPI_Get_count(&status, MPI_BYTE, &size);
	    std::vector<std::pair<K, V>> aux; aux.resize(size / sizeof(std::pair<K, V>));
	    int i = MPI_Recv(aux.data(), size, MPI_BYTE, proc, tag, comm, MPI_STATUS_IGNORE);
	    value.clear();
	    for(auto &it : aux) {
		value.insert(it);
	    }
	    return i;
	}
	
    };
    
};
