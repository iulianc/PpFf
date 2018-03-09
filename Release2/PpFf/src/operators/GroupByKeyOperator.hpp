#ifndef GROUPBYKEYOPERATOR_HPP
#define GROUPBYKEYOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <functional>

namespace PpFf{

	template < typename In, typename K, typename V, typename MapContainer, bool IsFuncOnValue >
	class GroupByKeyOperator: public FinalOperator {};


	template < typename In, typename K, typename V, typename MapContainer >
	class GroupByKeyOperator< In, K, V, MapContainer, false >: public FinalOperator {
	public:
		typedef MapContainer Value;
		GroupByKeyOperator(std::function< K*(In*) > const& taskFuncOnKey): taskFuncOnKey(taskFuncOnKey) { }
		GroupByKeyOperator(const GroupByKeyOperator& other) : taskFuncOnKey(other.taskFuncOnKey), mapContainer(other.mapContainer) { }
		GroupByKeyOperator(GroupByKeyOperator&& other) noexcept : taskFuncOnKey(std::move(other.taskFuncOnKey)), mapContainer(std::move(other.mapContainer)) { }
		GroupByKeyOperator& operator+= ( const GroupByKeyOperator& other ) {
			for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
				mapIt = mapContainer.find(otherIt->first);

				if(mapIt != mapContainer.end()){
					(mapIt->second).insert((mapIt->second).end(), (otherIt->second).begin(), (otherIt->second).end());
				}
				else{
					mapContainer[otherIt->first] = otherIt->second;
				}
			}

			return *this;
		}
		~GroupByKeyOperator() {
			delete key;
			key = NULL;
		};

		void* svc(void* task) {
			key = taskFuncOnKey((In*)task);
			mapContainer[*key].push_back(*((In*)task));

			return GO_ON;
		}

		MapContainer value(){
			return mapContainer;
		}

	private:
		std::function< K*(In*) > const& taskFuncOnKey;
		MapContainer mapContainer;
		K *key;
		typename MapContainer::iterator mapIt;
	};


	template < typename In, typename K, typename V, typename MapContainer >
	class GroupByKeyOperator< In, K, V, MapContainer, true >: public FinalOperator {
	public:
		typedef MapContainer Value;
		GroupByKeyOperator(std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue): taskFuncOnKey(taskFuncOnKey), taskFuncOnValue(taskFuncOnValue) { }
		GroupByKeyOperator(const GroupByKeyOperator& other) : taskFuncOnKey(other.taskFuncOnKey), taskFuncOnValue(other.taskFuncOnValue), mapContainer(other.mapContainer) { }
		GroupByKeyOperator(GroupByKeyOperator&& other) noexcept : taskFuncOnKey(std::move(other.taskFuncOnKey)), taskFuncOnValue(std::move(other.taskFuncOnValue)), mapContainer(std::move(other.mapContainer)) { }
		GroupByKeyOperator& operator+= ( const GroupByKeyOperator& other ) {
			for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
				mapIt = mapContainer.find(otherIt->first);

				if(mapIt != mapContainer.end()){
					(mapIt->second).insert((mapIt->second).end(), (otherIt->second).begin(), (otherIt->second).end());
				}
				else{
					mapContainer[otherIt->first] = otherIt->second;
				}
			}

			return *this ;
		}
		~GroupByKeyOperator() {
			delete key;
			delete val;
			key = NULL;
			val = NULL;
		};

		void* svc(void* task) {
			key = taskFuncOnKey((In*)task);
			val = taskFuncOnValue((In*)task);
			mapContainer[*key].push_back(*val);

			return GO_ON;
		}

		MapContainer value(){
			return mapContainer;
		}

	private:
		std::function< K*(In*) > const& taskFuncOnKey;
		std::function< V*(In*) > const& taskFuncOnValue;
		MapContainer mapContainer;
		K *key;
		V *val;
		typename MapContainer::iterator mapIt;
	};


}

#endif
