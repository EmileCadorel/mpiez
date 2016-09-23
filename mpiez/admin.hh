#pragma once

#include <list>
#include <vector>
#include <mpi.h>
#include <mpiez/stream.hh>
#include <mpiez/global.hh>
#include <mpiez/mpiexception.hh>


namespace mpiez {

    
    struct AdminMultipleDefinition : mpiezexception {
	AdminMultipleDefinition() : mpiezexception("cannot define multiple administrator") {
	}
    };
    
    template <class C, class P>
    struct admin {

	admin(int argc,char ** argv) {
	    if(nbAdmin == 0) {
		MPI_Init(&argc, &argv);
		int nprocs, id;
		MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
		MPI_Comm_rank(MPI_COMM_WORLD, &id);
	    
		m_protos.push_back(new P(id, nprocs));
		m_process.push_back(new C(*m_protos.back(), argc, argv));

		MPI_Barrier(MPI_COMM_WORLD);
		m_process.back()->routine();

		nbAdmin ++;
	    } else {
		throw AdminMultipleDefinition();
	    }
	}
	
	void finalize() {
	    MPI_Barrier(MPI_COMM_WORLD);
	    
	    m_process.back()->on_end();
	    
	    MPI_Finalize();
	    delete m_process.back();
	    delete m_protos.back();
	}




	std::list<C*> m_process;
	std::list<P*> m_protos;
	static int nbAdmin;
    };



    template <typename C, typename P>
    int admin<C, P>::nbAdmin = 0;
    
};

