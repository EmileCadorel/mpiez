#pragma once

#include <mpiez/vector.hh>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <mpiez/mpiexception.hh>



namespace mpiez {

    
    namespace ezsend {
	
	template <typename T>
	int send(int & proc, int & tag, mpiez::vector<T> & value, MPI_Comm comm) {
	    return MPI_Send(value.data(), value.size() * sizeof(T), MPI_BYTE, proc, tag, comm);
	}

	template <typename T>
	int ssend(int & proc, int & tag, mpiez::vector<T> & value, MPI_Comm comm) {
	    return MPI_Ssend(value.data(), value.size() * sizeof(T), MPI_BYTE, proc, tag, comm);
	}
	
    };

    namespace ezrecv {

	template<typename T>
	int recv(int & proc, int & tag, mpiez::vector<T> & value, MPI_Status & status, MPI_Comm comm) {
	    int size;
	    MPI_Probe(proc, tag, comm, &status);
	    MPI_Get_count(&status, MPI_BYTE, &size);
	    value.resize(size/sizeof(T));
	    return MPI_Recv(value.data(), size, MPI_BYTE, proc, tag, comm, MPI_STATUS_IGNORE);
	}
	
    };
    
    namespace ezsendrecv {

	template<typename T>
	int send_recv(int & to, int & from, int & tag, mpiez::vector<T> &to_send, mpiez::vector<T> & to_recv, MPI_Status & status, MPI_Comm comm) {

	    to_recv.resize(to_send.size());
	    
	    int i = MPI_Sendrecv(to_send.data(), to_send.size() * sizeof(T), MPI_BYTE, to, tag,
				 to_recv.data() , to_send.size() * sizeof(T), MPI_BYTE, from, tag,
				 comm, &status);
	    return i;
	}

	template<typename T>
	int send_recv_replace(int & to, int & from, int & tag, mpiez::vector<T> &to_send, MPI_Status & status, MPI_Comm comm) {
	    
	    return MPI_Sendrecv_replace(to_send.data(), to_send.size() * sizeof(T), MPI_BYTE, 
					 to, tag,
					 from, tag,
					 comm, &status);
	}


	
    };


    
};
