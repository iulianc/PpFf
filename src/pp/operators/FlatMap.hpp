

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <type_traits>
#include <ff/node.hpp>

using namespace ff;

//specialize a type for all of the STL containers.
namespace is_stl_container_impl{
  template <typename T>       struct is_stl_container:std::false_type{};
  template <typename T, std::size_t N> struct is_stl_container<std::array    <T,N>>    :std::true_type{};
  template <typename... Args> struct is_stl_container<std::vector            <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::deque             <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::list              <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::forward_list      <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::set               <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::multiset          <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::map               <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::multimap          <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_set     <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_multimap<Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::stack             <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::queue             <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::priority_queue    <Args...>>:std::true_type{};
}

//type trait to utilize the implementation type traits as well as decay the type
template <typename T> struct is_stl_container {
  static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};

template < typename In, typename Out, typename TaskFunc >
class FlatMap: public ff_node {
public:
	FlatMap(TaskFunc const& taskf): taskf(taskf){};
	~FlatMap(){};

	void* svc(void* task) {
		In* in = new In();
		*in = *(In*)task;
		return new Out(taskf(*in));
	}


private:
	TaskFunc const taskf;
};
