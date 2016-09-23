#include <mpiez/admin.hh>
#include <mpiez/protocol.hh>
#include <mpiez/process.hh>
using namespace mpiez;



struct proto : protocol {

    proto(int id, int total) : protocol(id, total) {
    }
   
    message<0, int> shift;
    message<1, int> bito;
};

struct session : process<proto> {

    session(proto & p, int argc, char ** argv) : process(p) {

	m_taille = atoi(argv[1]);
	root = atoi(argv[2]);
	m_n = m_taille * m_proto.total;
	m_data.resize(m_taille);
	
	if(m_proto.id < m_proto.total / 2) {
	    for (int i = 0; i < m_taille; i++)
		m_data[i] = m_proto.id * m_taille + (m_proto.id + 1) + i;
	} else {
	    for (int i = 0; i < m_taille; i++)
		m_data[i] = -(m_proto.id * m_taille + (m_proto.id + 1) + i + 1);
	}
	
	if(m_proto.id == 0) m_data[0] = -21;

	m_suite.resize(m_taille + 1);
	m_test.resize(m_taille + 1);
	m_suite[0] = 0;
	for(int i = 0; i < m_taille; i++) {
	    m_suite[i + 1] = m_data[i];
	}
    }



    void ShiftDroitCirculaire() {
	int to = (m_proto.id + 1) % m_proto.total;
	int from = (m_proto.id - 1) % m_proto.total;
	m_proto.shift.send_recv(to, from, m_suite.back(), m_suite.front());
	m_proto.shift.send_recv(to, from, m_test.back(), m_test.front());
    }
    

    void createTest() {
	for(int i = 1; i < m_suite.size(); i++) {
	    if(m_suite[i] >= m_suite[i - 1]) {
		m_test[i] = 1;
	    } else {
		m_test[i] = -1;
	    }
	}
    }
    
    int TestBitoniqueSimpleLocal() {
	if(m_proto.id != root) {
	    int res = m_test[0];
	    for(int j = 1; j < m_test.size(); j++) {
		if(m_test[j] == 1) {
		    if(res != 1) return -2;
		}
		else if(m_test[j] == -1) {
		    if(res == 1) res = 0;
		}
	    }
	    return res;
	} else {
	    int res = m_test[1];
	    for(int j = 2; j < m_test.size(); j++) {
		if(m_test[j] == 1) {
		    if(res != 1) return -2;
		}
		else if(m_test[j] == -1) {
		    if(res == 1) res = 0;
		}
	    }
	    return res;
	}

    }
 

    int TestBitoniqueSimple() {
	int i = TestBitoniqueSimpleLocal();
	mpiez::vector<int> in, out;
	in.resize(1);
	in[0] = i;
	
	global::gather(root, m_proto.total, in, out, MPI_COMM_WORLD);

	if(m_proto.id == root) {
	    std::cout << out << std::endl;

	    int res = out[0];
	    for(int i = 1; i < out.size(); i++) {
		if(res == 1){ res = out[i]; continue;}
		if(res == 0) {if (out[i] != -1) return -2;}
		if(res == -1) {if(out[i] != -1) return -2;}
		if(res == -2) {return -2;}
	    }
	    return res;
	}
    }


    void shiftTotal() {
	for(int i = m_suite.size()-1; i > 0; i--) {
	    m_suite[i] = m_suite[i - 1];
	}
    }

    
    
    int TestBitonique() {
	for(int i = 0; i < m_taille * m_proto.total; i++) {
	    createTest();
	    ShiftDroitCirculaire();
	    createTest();
	    global::sync_function([](int id, mpiez::vector<int> & m) { std::cout << id << " -> " << m << std::endl; }, m_proto.id, m_suite);
	    int res = TestBitoniqueSimple();
	    mpiez::vector<int> in, out;

	    if(m_proto.id == root) {
		in.resize(m_proto.total);
		for(int i = 0; i < m_proto.total; i++) {
		    in[i] = res;
		}
	    }
	    
	    global::scatter(root, m_proto.total, in, out, MPI_COMM_WORLD);

	    if(m_proto.id == 0) std::cout << std::endl << std::endl;
	    if(out[0] == -2) {
		shiftTotal();
	    } else {
		return out[0];
	    }
	}
	return -2;
    }
    
    
   
    void routine() {
	
	int bitonique = TestBitonique();

	
	global::sync_function([](int id, int bitonique) {
		std::cout << "sortie " << id << " -> " << bitonique << std::endl; }
	    , m_proto.id, bitonique);
	
  

    }
    
    void on_end() {
	
    }

    int m_taille, root;
    int m_n;
    mpiez::vector<int> m_data, m_suite, m_test;

    
};

int main(int argc, char ** argv) {
    try {
	mpiez::admin<session, proto> a(argc, argv);
	a.finalize();
    } catch(mpiezexception & e) {
	std::cout << e.text << std::endl;
	throw e;
    }
}
 
