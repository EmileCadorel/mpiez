#pragma once
#include <vector>
#include <list>

template <typename T>
std::ostream & operator<<(std::ostream & s, std::vector<T> & tab) {
    s << "[";
    for (int i = 0; i < tab.size(); i++) {
	s << tab[i];
	if(i < tab.size() - 1) s << ", ";
    }
    s << "]";
    return s;
}


template <typename T>
std::ostream & operator<<(std::ostream & s, std::list<T> & l) {
    s << "[";
    int i = 0;
    for(auto it : l) {
	s << it;
	if (i < l.size() - 1) s << ", ";
	i++;
    }
    s << "]";
    return s;
}

template <typename K, typename V>
std::ostream & operator<<(std::ostream & s, std::pair<K, V> & p) {
    s << "(" << p.first << " : " << p.second << ")";
    return s;
}


template <typename K, typename V>
std::ostream & operator<<(std::ostream & s, std::map<K, V> & m) {
    s << "{";
    int i = 0;
    for(auto it : m) {
	s << it;
	if (i < m.size() - 1) s << ", ";
	i++;
    }
    s << "}";
    return s;
}
