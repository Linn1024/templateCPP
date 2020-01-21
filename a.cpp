#include<iostream>
#include<vector>
#include <experimental/type_traits>
#include <tuple>
#include <utility>
#include <algorithm>


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
typename enable_if<hasIterator<T>, ostream&>::type operator<<(ostream& out, const T& a){
	counterOfCout++;
	int oldCounter = counterOfCout;
	for (auto iter = a.begin(); iter != a.end(); iter++){
		auto el = *iter;
		out << el;
		if (iter + 1 == a.end())
			break;
		if (counterOfCout - oldCounter == 0){
			out << " ";
			continue;
		}
		for (int i = 2; i < counterOfCout - oldCounter; i++)
			cout << endl;		
	}
	if (counterOfCout - oldCounter == 0){
		out << endl;
	}
	return out;
}

template <class T>
typename enable_if<hasIterator<T>, void>::type sort(T& a){
	sort(a.begin(), a.end());
}

template<class Ch, class Tr, class... Args>                                                      //cout tuple
auto& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {                 
	counterOfCout++;
	std::apply([&os](auto&&... args) {((os << args << " "), ...);}, t);
 	return os;
}

template<class T, class U>                                                                       //cout pair
ostream& operator<<(ostream& out, pair<T, U> p){
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

template<class T>                                                                                //two-dimensional array for more comfortable constructor
struct matrix : public vector<vector<T>>{
	using vector<vector<T>>::vector;
	matrix (size_t n = 0, size_t m = 0, T el = T()) : vector<vector<T>>(n, vector<T>(m, el)){
	}
};


template <typename T>                                                                            //function that prints any values with space delimeter
void print(const T& t)                                                                           //and make endl in the end (but it doesn't even matter) 
{
    std::cout << t << endl;
}

template<class T, class... Args>
void print(const T& el, Args... args)
{
	cout << el << " ";
	print(args...);
}


int main(){
}
