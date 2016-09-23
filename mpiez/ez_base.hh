#pragma once
#include <mpi.h>
#include <iostream>
#include <mpiez/admin.hh>

namespace mpiez {

    
    namespace ezsend {

	template <typename A>
	int send(int &proc, int &tag, A &value, MPI_Comm comm) {
	    return MPI_Send(&value, sizeof(value), MPI_BYTE, proc, tag, comm);
	}


	template <typename A>
	int ssend(int &proc, int &tag, A &value, MPI_Comm comm) {
	    return MPI_Ssend(&value, sizeof(value), MPI_BYTE, proc, tag, comm);
	}
	
	int send(int & proc, int & tag, std::string value, MPI_Comm comm) {
	    char * c = new char[value.size()];
	    for(int i = 0; i < value.size(); i++) {
		c[i] = value[i];
	    }
	    int i = MPI_Send(c, value.size(), MPI_CHAR, proc, tag, comm);
	    delete c;
	    return i;
	}

	int send(int &proc, int &tag, char * value, MPI_Comm comm) {
	    return MPI_Send(value, strlen(value), MPI_CHAR, proc, tag, comm);
	}

	int ssend(int & proc, int & tag, std::string value, MPI_Comm comm) {
	    char * c = new char[value.size()];
	    for(int i = 0; i < value.size(); i++) {
		c[i] = value[i];
	    }
	    int i = MPI_Ssend(c, value.size(), MPI_CHAR, proc, tag, comm);
	    delete c;
	    return i;
	}

	int ssend(int &proc, int &tag, char * value, MPI_Comm comm) {
	    return MPI_Ssend(value, strlen(value), MPI_CHAR, proc, tag, comm);
	}

	
	
	
    };


    namespace ezrecv {

	template <typename T>
	int recv(int & proc, int & tag, T & value, MPI_Status & status, MPI_Comm comm) {
	    return MPI_Recv(&value, sizeof(T), MPI_BYTE, proc, tag, comm, &status);
	}

	template<>
	int recv(int & proc, int & tag, std::string & value, MPI_Status & status, MPI_Comm comm) {
	    int size;
	    MPI_Probe(proc, tag, comm, &status);

	    MPI_Get_count(&status, MPI_CHAR, &size);

	    char* buf = new char[size];

	    int i = MPI_Recv(buf, size, MPI_CHAR, proc, tag, comm, MPI_STATUS_IGNORE);
	    value = buf;
	    delete buf;
	    return i;
	}



	
    };


    namespace ezsendrecv {

	template <typename T, typename T2>
	int send_recv(int & to, int & from, int & tag, T to_send, T2 & to_recv, MPI_Status & status, MPI_Comm comm) {
	    return MPI_Sendrecv(&to_send, sizeof(T), MPI_BYTE, to, tag,
				&to_recv, sizeof(T2), MPI_BYTE, from, tag,
				comm, &status);
	}
	
	int send_recv(int & to, int & from, int & tag, std::string to_send, std::string & to_recv,MPI_Status & status, MPI_Comm comm) {
	    char * c = new char[to_send.size()];
	    to_send.copy(c,0, to_send.size() - 1); 
	    char * buf = new char[to_send.size()];
	    
	    int i = MPI_Sendrecv(c, to_send.size(), MPI_CHAR, to, tag,
				buf , to_send.size(), MPI_CHAR, from, tag,
				comm, &status);
	    to_recv = buf;
	    delete buf;
	    delete c;
	    return i;
	}

	int send_recv(int &to, int & from, int &tag, char * to_send, std::string & to_recv, MPI_Status & status, MPI_Comm comm) {
	    int size = strlen(to_send);
	    char * buf = new char[size];
	    
	    int i = MPI_Sendrecv(to_send, size, MPI_CHAR, to, tag,
				 buf , size, MPI_CHAR, from, tag,
				 comm, &status);
	    to_recv = buf;
	    delete buf;
	    return i;
	}


	template <typename T, typename T2>
	int send_recv_replace(int & to, int & from, int & tag, T & to_send, MPI_Status & status, MPI_Comm comm) {
	    return MPI_Sendrecv_replace(&to_send, sizeof(T), MPI_BYTE, to, tag, from, tag, comm, &status);
	}
	
	int send_recv_replace(int & to, int & from, int & tag, std::string & to_send, MPI_Status & status, MPI_Comm comm) {
	    char * c = new char[to_send.size()];
	    to_send.copy(c,0, to_send.size() - 1); 
	    
	    int i = MPI_Sendrecv_replace(c, to_send.size(), MPI_CHAR, to, tag,
					 from, tag,
					 comm, &status);
	    to_send = c;
	    delete c;
	    return i;
	}

	
	
    };

};
