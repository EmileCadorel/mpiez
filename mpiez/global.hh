#pragma once

#include <mpiez/mpiexception.hh>
#include <mpi.h>
#include <mpiez/vector.hh>
#include <iostream>
#include <string.h>



namespace mpiez {

    namespace global {

	void barrier(MPI_Comm comm) {
	    MPI_Barrier(comm);
	}

	template <typename T>
	void scatter(int root, int size, mpiez::vector<T> & in, mpiez::vector<T> & out, MPI_Comm comm) {
	    int nb_procs, id;
	    MPI_Comm_size(comm, &nb_procs);
	    MPI_Comm_rank(comm, &id);
	    int n_s = size / nb_procs;
	    int reste = size % nb_procs;
	    mpiez::vector<int> ind, displs;
	    if(id == root) {
		
		ind.resize(nb_procs);
		displs.resize(nb_procs);
		
		int disp = 0;
		for(int i = 0; i < ind.size(); i++) {
		    ind[i] = n_s * sizeof(T);
		    if(i < reste) ind[i] += sizeof(T);
		    displs[i] = disp;
		    disp += ind[i];
		}
	    }
	    int size_ = n_s;
	    if(id < reste) {
		size_ += 1;
	    }

	    out.resize(size_);

	    MPI_Scatterv(in.data(), ind.data(), displs.data(), MPI_BYTE, out.data(), size_ * sizeof(T), MPI_BYTE, root, comm);
	}

	template <typename T>
	void scatter(int root, int size, mpiez::vector<mpiez::vector<T> > & in, mpiez::vector<mpiez::vector<T> > & out, MPI_Comm comm) {
	    std::cout << "Send of multiple vector<vector<...>>> can't work" << std::endl; 
	}

	
	template <typename T>
	void scatter_nxm(int root, int n, int m, mpiez::vector<T> & in, mpiez::vector<T> & out, MPI_Comm comm) {
	    int nb_procs, id;
	    MPI_Comm_size(comm, &nb_procs);
	    MPI_Comm_rank(comm, &id);
	    int n_s = n / nb_procs;
	    int reste = n % nb_procs;

	    mpiez::vector<int> ind, displs;
	    if(id == root) {
		
		ind.resize(nb_procs);
		displs.resize(nb_procs);

		int disp = 0;
		for(int i = 0; i < ind.size(); i++) {
		    ind[i] = n_s * sizeof(T) * m;
		    if(i < reste) ind[i] += sizeof(T) * m;
		    displs[i] = disp;
		    disp += ind[i];
		}
	    }
	    
	    int size_ = n_s * m;
	    if(id < reste) {
		size_ += m;
	    }
	    
	    
	    out.resize(size_);


	    MPI_Scatterv(in.data(), ind.data(), displs.data(), MPI_BYTE, out.data(), size_ * sizeof(T), MPI_BYTE, root, comm);
	}

	template <typename T>
	void scatter_nxm(int, int, int, mpiez::vector<mpiez::vector<T> > &, mpiez::vector<mpiez::vector<T> > & , MPI_Comm) {
	    throw mpiezexception("Send of vector<vector<...>>> can't work");
	}
	
	template <typename T>
	void gather(int root, int size, mpiez::vector<T> & in, mpiez::vector<T> & out, MPI_Comm comm) {
	    int nb_procs, id;
	    MPI_Comm_size(comm, &nb_procs);
	    MPI_Comm_rank(comm, &id);
	    int n_s = size / nb_procs;
	    int reste = size % nb_procs;
	    mpiez::vector<int> ind, displs;
	    if(id == root) {
		
		ind.resize(nb_procs);
		displs.resize(nb_procs);
		
		int disp = 0;
		for(int i = 0; i < ind.size(); i++) {
		    ind[i] = n_s * sizeof(T);
		    if(i < reste) ind[i] += sizeof(T);
		    displs[i] = disp;
		    disp += ind[i];
		}
		out.resize(size);

	    }
	   
	    MPI_Gatherv(in.data(), in.size() * sizeof(T), MPI_BYTE, out.data(), ind.data(), displs.data(), MPI_BYTE, root, comm);
	}
	
	template <typename T>
	void reduce(int root, int size, mpiez::vector<T> & in, mpiez::vector<T> & out, MPI_Datatype type, MPI_Op op, MPI_Comm comm) {
	    int id;
	    MPI_Comm_rank(comm, &id);
	    if(id == root) {
		out.resize(in.size());
	    }
	    
	    MPI_Reduce(in.data(), out.data(), size, type, op, root, comm);  
	}
	
	template <typename T>
	void all_reduce(int size, mpiez::vector<T> & in, mpiez::vector<T> & out, MPI_Datatype type, MPI_Op op, MPI_Comm comm) {
	    out.resize(in.size());
	    MPI_Allreduce(in.data(), out.data(), size, type, op, comm);  
	}


	void dims_create(int total, int nb_dim, int size[2]) {
	    MPI_Dims_create(total, nb_dim, size);
	}

	void cart_create(MPI_Comm comm, int nb_dim, int size[2], int periodics[2], int reorder, MPI_Comm & out_comm) {
	    MPI_Cart_create(comm, nb_dim, size, periodics, reorder, &out_comm);
	}

	void cart_coords(MPI_Comm comm, int pid, int nb_dims, int coords[2]) {
	    MPI_Cart_coords(comm, pid, nb_dims, coords);
	}

	void cart_coords(MPI_Comm comm, int pid, int nb_dims, mpiez::vector<int> &coords) {
	    coords.resize(2);
	    MPI_Cart_coords(comm, pid, nb_dims, coords.data());
	}

	namespace shift {
	    enum type {
		VER = 0, HOR
	    };
	};
	
	void cart_shift(MPI_Comm comm, shift::type type, int nb, int & src, int & dest) {
	    MPI_Cart_shift(comm, type, nb, &src, &dest);
	}
	
	template<typename Foo, typename ... Params>
	void sync_function(MPI_Comm comm, Foo f, Params &... param) {
	    int id, size;

	    MPI_Comm_rank(comm, &id);
	    MPI_Comm_size(comm, &size);
	    for(int i = 0; i < size; i++) {
		if(id == i) {
		    f(param...);
		}
		barrier(comm);
	    }
	    barrier(comm);
	}


	template<typename Foo, typename ... Params>
	void sync_function(Foo f, Params& ... param) {
	    int id, size;

	    MPI_Comm_rank(MPI_COMM_WORLD, &id);
	    MPI_Comm_size(MPI_COMM_WORLD, &size);
	    for(int i = 0; i < size; i++) {
		if(id == i) {
		    f(param...);
		}
		barrier(MPI_COMM_WORLD);
	    }
	    barrier(MPI_COMM_WORLD);
	}
	

	



    };

};
