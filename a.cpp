#include<iostream>
#include<vector>
#include <experimental/type_traits>
#include <tuple>
#include <utility>

using namespace std;


#define DEBUG

#ifndef DEBUG
#define cerr cerr1
#define endl '\n'
struct emptyClass{                                                                               //remove cerr in release (it works fast also)
};                                                                                               //don't use with functions (compiler doesn't know if function changes state of variables) 
                                                                                                 //                         (it still doesn't print but functions run)
template <class T>
emptyClass& operator<<(emptyClass& cl, T& t){
    return cl;
}
 
emptyClass cerr;
#define cerr cerr1
#endif

#ifdef DEBUG                                                                                     //check vector range in debug
template <class T>
struct vector1 : public vector<T>{
    using vector<T>::vector;
	auto operator[](size_t t) -> decltype(vector<T>().at(t)){
		return (*this).at(t);
	}
	auto operator[](size_t t)  const -> decltype(vector<T>().at(t)){
		return (*this).at(t);
	}
};
#endif

int counterOfCout = 0;

template<typename T>                                                                             //cout everything that has iterator begin()
using hasIterator_t = decltype( std::declval<T&>().begin());

template<typename T>
constexpr bool hasIterator = std::experimental::is_detected<hasIterator_t, T>::value;

template <class T>
typename enable_if<hasIterator<T>, ostream&>::type operator<<(ostream& out, T a){
	counterOfCout++;
	int oldCounter = counterOfCout;
	for (const auto& el : a){
		out << el << " ";
		if (counterOfCout - oldCounter != 0){
			out << endl;
		}
	}
	return out;
}

template<class Ch, class Tr, class... Args>                                                      //cout tuple
auto& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {                 
  counterOfCout++;
  std::apply([&os](auto&&... args) {((os << args << " "), ...);}, t);
  return os;
}

template<class T, class U>                                                                       //cout pair
auto& operator<<(ostream& out, pair<T, U> p){
	counterOfCout++;
	return (out << p.first << " " << p.second);
}
                                                                                                 
template<class T>                                                                                //cin vector (any dimension)
istream& operator>>(istream& in, vector<T>& v){
	for (auto& e : v){
		in >> e;
	}
	return in;
}

int main(){
}