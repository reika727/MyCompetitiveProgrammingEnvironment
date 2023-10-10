#include"bits/stdc++.h"
using namespace std;
using ll=long long;
template<class T=ll>inline T in(istream&is=cin){return*istream_iterator<T>(is);}
template<class Container=vector<ll>>inline Container ins(istream&is=cin){return Container(istream_iterator<typename Container::value_type>(is),istream_iterator<typename Container::value_type>());}
template<class Container=vector<ll>>inline Container ins(size_t n,istream&is=cin){Container c;copy_n(istream_iterator<typename Container::value_type>(is),n,back_inserter(c));return c;}
template<class T>inline bool chmin(T&a,T b){return a>b?a=b,true:false;}
template<class T>inline bool chmax(T&a,T b){return a<b?a=b,true:false;}
template<class T,class...Sizes>inline auto multdim_vector_v(T v,size_t size,Sizes...sizes){if constexpr(sizeof...(Sizes)==0)return vector(size,v);else return vector(size,multdim_vector_v(v,sizes...));}
template<class T,class...Sizes>inline auto multdim_vector(Sizes...sizes){return multdim_vector_v(T{},sizes...);}

int main()
{
}
