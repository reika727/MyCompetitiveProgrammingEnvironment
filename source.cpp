#include"bits/stdc++.h"
using namespace std;
using ll=long long;
template<class T=ll>inline T in(istream&is=cin){return*istream_iterator<T>(is);}
template<class T=ll,template<class,class>class Container=vector,template<class>class Allocator=allocator>inline Container<T,Allocator<T>>ins(istream&is=cin){return{istream_iterator<T>(is),istream_iterator<T>()};}
template<class Container>concept pushbackable=requires(Container&c){c.push_back(declval<typename Container::value_type>());};
template<class Container>concept reservable=requires(Container&c){c.reserve();};
template<class T=ll,template<class,class>class Container=vector,template<class>class Allocator=allocator>requires pushbackable<Container<T,Allocator<T>>>inline Container<T,Allocator<T>>ins(size_t n,istream&is=cin){Container<T,Allocator<T>>c;if constexpr(reservable<decltype(c)>)c.reserve(n);copy_n(istream_iterator<T>(is),n,back_inserter(c));return c;}
template<class T>inline bool chmin(T&a,T b){return a>b?a=b,true:false;}
template<class T>inline bool chmax(T&a,T b){return a<b?a=b,true:false;}
template<class T,class...Sizes>inline auto multdim_vector_v(T v,size_t size,Sizes...sizes){if constexpr(sizeof...(Sizes)==0)return vector(size,v);else return vector(size,multdim_vector_v(v,sizes...));}
template<class T,class...Sizes>inline auto multdim_vector(Sizes...sizes){return multdim_vector_v(T{},sizes...);}
namespace{
#ifdef ONLINE_JUDGE
constexpr auto is_online_judge=true;
#else
constexpr auto is_online_judge=false;
#endif
[[maybe_unused]]constexpr struct dbg_stream final{
inline const dbg_stream&operator<<([[maybe_unused]]decltype(std::cerr)&(*pf)(decltype(std::cerr)&))const{if constexpr(!is_online_judge)pf(std::cerr);return*this;}
inline const dbg_stream&operator<<([[maybe_unused]]const auto&v)const{if constexpr(!is_online_judge)std::cerr<<v;return*this;}
}dbg;
}

int main()
{
}
