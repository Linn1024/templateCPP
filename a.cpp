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

int depth = 0;
int maxDepth = 0;

template<typename T>                                                                             //cout everything that has iterator begin()
using hasIterator_t = decltype( std::declval<T&>().begin());

template<typename T>
constexpr bool hasIterator = std::experimental::is_detected<hasIterator_t, T>::value;

template <class T>
typename enable_if<hasIterator<T>, ostream&>::type operator<<(ostream& out, const T& a){
	depth++;
	int myDepth = depth;
	maxDepth = max(maxDepth, depth);
	for (auto iter = a.begin(); iter != a.end(); iter++){
		auto el = *iter;
		out << el;
		if (iter + 1 == a.end())
			break;
		if (maxDepth - myDepth == 0){
			out << " ";
			continue;
		}
		for (int i = 0; i < maxDepth - myDepth; i++)
			cout << endl;		
	}
	if (myDepth == 1){
		out << endl;
	}
	depth--;
	if (myDepth == 1)
		maxDepth = 0;
	return out;
}

template <class T>
typename enable_if<hasIterator<T>, void>::type sort(T& a){                                       //sort whole container
	sort(a.begin(), a.end());
}

template<class Ch, class Tr, class... Args>                                                      //cout tuple
auto& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {                 
	maxDepth = max(depth + 1, maxDepth);
	std::apply([&os](auto&&... args) {((os << args << " "), ...);}, t);
 	return os;
}

template<class T, class U>                                                                       //cout pair
ostream& operator<<(ostream& out, const pair<T, U>& p){
	maxDepth = max(depth + 1, maxDepth);
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
	matrix (size_t n = 0, size_t m = 0, T el = T()) : vector<vector<T>>(n, vector<T>(m, el)){};
	void resize(size_t n, size_t m, T el = T()){
		vector<vector<T>>::resize(n, vector<T>(m, el));
	}
};


template <ostream& out=cout, typename T>                                                         //function that prints any values with space delimeter
void print(const T& t)                                                                           //and make endl in the end (but it doesn't even matter) 
{                                                                                                //e.g print(1, "plus", 2, '=', 3);
	out << t << endl;                                                                        //1 plus 2 = 3
}                                                                                                //use template for another ostream
                                                                                                 //e.g. print<cerr>("I am here")
template<ostream& out=cout, class T, class... Args>
void print(const T& el, Args... args)
{
	out << el << " ";
	print(args...);
}


int main(){
	cin.tie(NULL);
	ios_base::sync_with_stdio(false);
}
