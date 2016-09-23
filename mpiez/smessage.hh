#pragma once

#include <mpiez/ez_base.hh>
#include <mpiez/ez_vector.hh>

namespace mpiez {
    
    template <int N, typename ... T>
    struct smessage {

	void send(int procId, T ... params) {
	    send_(procId, params...);
	}   

	void recv(int procId, T &... params) {
	    recv_(procId, params...);
	}

	void setComm(MPI_Comm comm) {
	    m_comm = comm;
	}

	MPI_Comm getComm() {
	    return m_comm;
	}
	
	int m_value = N;
	
	
    private:
	
	MPI_Status m_status;
	MPI_Comm m_comm = MPI_COMM_WORLD;
	
	template <typename T_, typename ... TArgs>
	void send_(int procId, T_ param, TArgs ... params) {
	    ezsend::ssend(procId, m_value ,param, m_comm);
	    send_(procId, params...);
	}
	
	void send_(int procId) {
	}

	template <typename T_, typename ... TArgs>
	void recv_(int procId, T_ & param, TArgs& ... params) {
	    ezrecv::recv(procId, m_value, param, m_status, m_comm);
	    recv_(procId, params...);
	}

	
	void recv_(int procId) {
	}
	
	
    };
};
