#pragma once

namespace mpiez {

    template < typename X > struct is_not_template : std::true_type {} ;
    
    template < template <typename...> class X, typename... P >
    struct is_not_template< X<P...> > : std::false_type {} ;

};
