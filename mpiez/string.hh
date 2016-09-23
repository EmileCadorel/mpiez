#pragma once

#include <mpiez/mpiexception.hh>
#include <string.h>

namespace mpiez {

    struct string {

	string() : m_size(0), m_data(NULL) {
	}
	
	string(const char * s) {
	    copy(s);
	} 

	string(const string & other) {
	    m_data = new char[other.m_size];
	    m_size = other.m_size;
	    for(int i = 0; i < m_size; i++) {
		m_data[i] = other.m_data[i];
	    }
	}

	int size() const {
	    return m_size;
	}
	
	void copy(const char * s) {
	    m_size = strlen(s);
	    m_data = new char[m_size];
	    for(int i = 0; i < m_size ; i++) {
		m_data[i] = s[i];
	    }
	}

	void clear() {
	    if(m_size != 0) {
		delete m_data;
		m_size = 0;
		m_data = NULL;
	    }
	}

	string operator+(const string & o) {
	    string s;
	    s.m_data = new char[m_size + o.m_size];
	    s.m_size = m_size + o.m_size;
	    int i = 0;
	    for(; i < m_size;i++) {
		s.m_data[i] = m_data[i];
	    }
	    for(; i < s.m_size; i++) {
		s.m_data[i] = o.m_data[i - m_size];
	    }
	    return s;
	}
	
	string operator+(const char * s) {
	    string str;
	    int s_size = strlen(s);
	    
	    str.m_data = new char[m_size + s_size];
	    str.m_size = m_size + s_size;

	    int i = 0;

	    for(; i < m_size;i++) {
		str.m_data[i] = m_data[i];
	    }
	    
	    for(; i < str.m_size; i++) {
		str.m_data[i] = s[i - m_size];
	    }
	    
	    return str;
	}

	string & operator+=(const string & o) {
	    char* s;
	    s = new char[m_size + o.m_size];
	    int i = 0;

	    for(; i < m_size; i++) {
		s[i] = m_data[i];
	    }
	    
	    for(; i < m_size + o.m_size; i++) {
		s[i] = o.m_data[i - m_size];
	    }
	    m_size = m_size + o.m_size;
	    
	    delete m_data;
	    m_data = s;

	    return *this;
	}

	string & operator+=(const char* o) {
	    char* s;
	    int s_size = strlen(o);
	    s = new char[m_size + s_size];
	    int i = 0;

	    for(; i < m_size; i++) {
		s[i] = m_data[i];
	    }

	    for(; i < m_size + s_size; i++) {
		s[i] = o[i - m_size];
	    }
	    m_size = m_size + s_size;	    
	    delete m_data;
	    m_data = s;

	    return *this;
	}
	
	
	char & operator[](int index) {
	    if(m_size != 0) {
		index = index % m_size;
		return m_data[index];
	    } else {
		throw out_of_index(index);
	    }
	}

	char operator[](int index) const {
	    if(m_size != 0) {
		index = index % m_size;
		return m_data[index];
	    } else {
		throw out_of_index(index);
	    }
	}
	
	char * data() {
	    return m_data;
	}
	
	friend std::ostream & operator<<(std::ostream & s, const string & str) {
	    for(int i = 0; i < str.size(); i++) {
		s << str[i];
	    }
	    return s;
	}
	
	
	
    private:

	char * m_data;
	int m_size;
	
    };

};
