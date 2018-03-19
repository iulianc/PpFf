#ifndef REDUCEBYKEYOPERATOR_HPP
#define REDUCEBYKEYOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <operators/Reducing.hpp>
#include <functional>

namespace PpFf{

	template < typename In, typename K, typename V, typename MapContainer, bool IsFuncOnKey>
	class ReduceByKeyOperator: public FinalOperator {};


	template < typename In, typename K, typename V, typename MapContainer >
	class ReduceByKeyOperator< In, K, V, MapContainer, false >: public FinalOperator {
	public:
		typedef MapContainer Value;
		ReduceByKeyOperator(Reducing< In, V > const& reducing): reducing(reducing) { }
		ReduceByKeyOperator(const ReduceByKeyOperator& other) : reducing(other.reducing) { }
		ReduceByKeyOperator(ReduceByKeyOperator&& other) noexcept : reducing(std::move(other.reducing)) { }
		ReduceByKeyOperator& operator+= ( ReduceByKeyOperator& other ) {
			for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
				mapIt = mapContainer.find(otherIt->first);

				if(mapIt != mapContainer.end()){
					if(reducing.isCombiner){
						reducing.combiner(&(mapIt->second), &(otherIt->second));
					}
				}
				else{
					mapContainer[otherIt->first] = otherIt->second;
				}
			}

			return *this;
		}
		~ReduceByKeyOperator() { };

		void* svc(void* task) {
			mapIt = mapContainer.find(*((In*)task));

			if (mapIt != mapContainer.end()){
				reducing.accumulator(&(mapIt->second), (In*)task);
			}else{
				mapContainer[*((In*)task)] = val;
				reducing.accumulator(&(mapContainer[*((In*)task)]), (In*)task);
			}

			return GO_ON;
		}

		MapContainer value(){
			return mapContainer;
		}

	private:
		Reducing< In, V > const& reducing;
		V val = reducing.identity;
		MapContainer mapContainer;
		typename MapContainer::iterator mapIt;
	};



	template < typename In, typename K, typename V, typename MapContainer >
	class ReduceByKeyOperator< In, K, V, MapContainer, true >: public FinalOperator {
	public:
		typedef MapContainer Value;
		ReduceByKeyOperator(std::function< K*(In*) > const& taskFuncOnKey, Reducing< In, V > const& reducing): taskFuncOnKey(taskFuncOnKey), reducing(reducing) { }
		ReduceByKeyOperator(const ReduceByKeyOperator& other) : taskFuncOnKey(other.taskFuncOnKey), reducing(other.reducing) { }
		ReduceByKeyOperator(ReduceByKeyOperator&& other) noexcept : taskFuncOnKey(std::move(other.taskFuncOnKey)), reducing(std::move(other.reducing)) { }
		ReduceByKeyOperator& operator+= ( ReduceByKeyOperator& other ) {
			for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
				mapIt = mapContainer.find(otherIt->first);

				if(mapIt != mapContainer.end()){
					if(reducing.isCombiner){
						reducing.combiner(&(mapIt->second), &(otherIt->second));
					}
				}
				else{
					mapContainer[otherIt->first] = otherIt->second;
				}
			}

			return *this;
		}
		~ReduceByKeyOperator() { };

		void* svc(void* task) {
			key = taskFuncOnKey((In*)task);
			mapIt = mapContainer.find(*key);

			if (mapIt != mapContainer.end()){
				reducing.accumulator(&(mapIt->second), (In*)task);
			}else{
				mapContainer[*key] = val;
				reducing.accumulator(&(mapContainer[*key]), (In*)task);
			}

			return GO_ON;
		}

		MapContainer value(){
			return mapContainer;
		}

	private:
		std::function< K*(In*) > const& taskFuncOnKey;
		Reducing< In, V > const& reducing;
		V val = reducing.identity;
		MapContainer mapContainer;
		typename MapContainer::iterator mapIt;
		K *key;
	};

}

#endif
