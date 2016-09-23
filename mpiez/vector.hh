#pragma once

#include <type_traits>
#include <mpiez/mpiexception.hh>
#include <mpiez/is_template.hh>

namespace mpiez {

    template <typename T>
    struct vector {

	static_assert(std::is_fundamental<T>::value, "error message");

	vector(unsigned int size = 0) : m_data(NULL), m_size(0) {
	    resize(size);
	}

	vector(const vector<T> & o) : m_data(NULL), m_size(0) {
	    resize(o.size());
	    for(int i = 0; i < o.size(); i++) {
		m_data[i] = o[i];
	    }
	}
	
	vector(std::initializer_list<T> f) : m_data(NULL), m_size(0) {
	    resize(f.size());
	    int i = 0;
	    for(auto it : f) {
		m_data[i] = it;
		i++;
	    }
	}

	vector & operator=(std::initializer_list<T> f) {
	    if(m_size != 0) delete m_data; m_data = NULL;
	    resize(f.size());
	    int i = 0;
	    for(auto it : f) {
		m_data[i] = it;
		i++;
	    }
	    return *this;
	}

	
	~vector() {
	    if(m_size > 0) {
		delete [] m_data;
	    }
	}
	
	int resize(unsigned int size) {
	    if(size == 0) {
		m_size = size;
		if(m_data) {
		    delete [] m_data;
		}
		m_data = NULL;
	    } else {
		
		T * aux = new T[size];
		if(m_data) {
		    for(int i = 0; i < m_size; i++) {
			aux[i] = m_data[i];
		    }
		}
		m_data = aux;
		m_size = size;
	    }
	}

	int size() const {
	    return m_size;
	}
	
	void clear() {
	    delete m_data;
	    m_data = NULL;
	    m_size = 0;
	}

	T & operator[](int index) {
	    if(m_size != 0) {
		index = index % m_size;
		return m_data[index];
	    } else {
		throw out_of_index(index);
	    }
	}

	T operator[](int index) const {
	    if(m_size != 0) {
		index = index % m_size;
		return m_data[index];
	    } else {
		throw out_of_index(index);
	    }
	}
	
	T & back() {
	    if(m_size != 0) {
		return m_data[m_size - 1];
	    } else {
		throw out_of_index(0);
	    }
	}

	T & front() {
	    if(m_size != 0) {
		return m_data[0];
	    } else {
		throw out_of_index(0);
	    }
	}
	

	T * data() {
	    return m_data;
	}

	friend std::ostream & operator<<(std::ostream & s, const vector<T> & t) {
	    s << "[";
	    for(int i = 0; i < t.size(); i++) {
		s << t[i];
		if(i < t.size() - 1) s << ", ";
	    }
	    s << "]";
	    return s;
	}

	
    private:

	T * m_data;
	int m_size;
	
    };

    
};

