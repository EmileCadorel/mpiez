#pragma once

#include <mpiez/ez_base.hh>
#include <mpiez/ez_vector.hh>
#include <mpiez/ez_list.hh>
#include <mpiez/ez_map.hh>

namespace mpiez {
    
	
    template <int N, typename ... T>
    struct message {

	void send(int procId, T &... params) {
	    send_(procId, params...);
	}   

	void recv(int procId, T &... params) {
	    recv_(procId, params...);
	}

	void send_recv(int procId, T &... params, T&... params2) {
	    send_recv_(procId, params..., params2...);
	}
	
	void send_recv(int procId1, int procId2, T &... params, T&... params2) {
	    send_recv_(procId1, procId2, params..., params2...);
	}

	void send_recv_replace(int procId, T &... params) {
	    send_recv_replace_(procId, params...);
	}

	void send_recv_replace(int procId1, int procId2, T &... params) {
	    send_recv_replace_(procId1, procId2, params...);
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
	void send_(int procId, T_ &param, TArgs &... params) {
	    ezsend::send(procId, m_value ,param, m_comm);
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

	template <typename T_, typename ... TArgs>
	void send_recv_(int procId, T_ param, TArgs ...params, T_ & param2, TArgs &... params2) {
	    ezsendrecv::send_recv(procId, procId, m_value, param, param2, m_status, m_comm);
	    send_recv_(procId, params..., params2...);
	}

	void send_recv_(int ) {
	}

	
	template <typename T_, typename ... TArgs>
	void send_recv_(int procId1, int procId2, T_& param, TArgs& ...params, T_ & param2, TArgs &... params2) {
	    ezsendrecv::send_recv(procId1, procId2, m_value, param, param2, m_status, m_comm);
	    send_recv_(procId1, procId2, params..., params2...);
	}

	void send_recv_(int, int) {
	}
	

	template<typename T_, typename ... TArgs>
	void send_recv_replace_(int procId, T_ &param, TArgs&... params) {
	    ezsendrecv::send_recv_replace(procId, procId, m_value, param, m_status, m_comm);
	    send_recv_replace_(procId, params...);
	}

	void send_recv_replace_(int ) {
	}


	template<typename T_, typename ... TArgs>
	void send_recv_replace_(int procId1, int procId2, T_& param, TArgs&... params) {
	    ezsendrecv::send_recv_replace(procId1, procId2, m_value, param, m_status, m_comm);
	    send_recv_replace_(procId1, procId2, params...);
	}

	void send_recv_replace_(int, int) {
	}
	
	
    };


};
