#pragma once
#include <memory>
#include <map>
#include <unordered_map>
#include <string>


namespace K3D12 {
	//シンプルなリソースキャリアーのクラス
	template <class T>
	class ManagerComponentBase
	{
	private:

		std::map<std::string, std::shared_ptr<T>> _resourceMap;

	protected:
	
	public:
	
	private:
	
	public:

		ManagerComponentBase() {};

		virtual ~ManagerComponentBase() { DiscardMap(); };
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name, std::shared_ptr<T> resource, bool forceOverride = false);
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name, const T& resource, bool forceOverride = false);
		//要素の取得
		std::weak_ptr<T>	 Get(std::string name);
		//要素の削除
		void Erase(std::string name);
		//マップの明示的クリア
		void DiscardMap();
		//リソースがロード済みか否か
		bool IsLooded(std::string name);
		//マップ自体の参照を取得する
		std::map<std::string, std::shared_ptr<T>>& GetMap();


	};


	template <class T>
	class NonRaiiManagerComponentBase
	{
	private:

	protected:

		std::map<std::string, T> _resourceMap;

	public:

	private:

	public:

		NonRaiiManagerComponentBase() {};

		virtual ~NonRaiiManagerComponentBase() { DiscardMap(); };
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name,const T& resource, bool forceOverride = false);
		//要素の取得
		T*	 Get(std::string name);
		//要素の削除
		void Erase(std::string name);
		//マップの明示的クリア
		void DiscardMap();
		//リソースがロード済みか否か
		bool IsLooded(std::string name);
		//マップ自体の参照を取得する
		std::map<std::string, T>& GetMap();


	};

	template <class T>
	class UnorderedManagerComponentBase
	{
	private:

	protected:

		std::unordered_map<std::string, std::shared_ptr<T>> _resourceMap;

	public:

	private:

	public:	
		
		UnorderedManagerComponentBase() {};

		virtual ~UnorderedManagerComponentBase() { DiscardMap(); };
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name, std::shared_ptr<T> resource, bool forceOverride = false);
		//要素の追加。第三引き数をTrueで強制上書き
		void Set(std::string name, const T & resource, bool forceOverride = false);
		//要素の取得
		std::weak_ptr<T>  Get(std::string name);
		//要素の削除
		void Erase(std::string name);
		//マップの明示的クリア
		void DiscardMap();
		//リソースがロード済みか否か
		bool IsLooded(std::string name);
		//マップ自体の参照を取得する
		std::unordered_map<std::string, std::shared_ptr<T>>& GetMap();
	};
}

template<class T>
inline void K3D12::ManagerComponentBase<T>::Set(std::string name, std::shared_ptr<T> resource, bool forceOverride)
{

	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = resource;

	}
}

template<class T>
inline void K3D12::ManagerComponentBase<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::make_shared<T>(resource);
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = std::make_shared<T>(resource);

	}
}

template<class T>
inline std::weak_ptr<T> K3D12::ManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return _resourceMap[name];
	}
	return std::weak_ptr<T>();
}

template<class T>
inline void K3D12::ManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D12::ManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();
}

template<class T>
inline bool K3D12::ManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::map<std::string, std::shared_ptr<T>>& K3D12::ManagerComponentBase<T>::GetMap()
{

	return _resourceMap;
}


//NoRaii


template<class T>
inline void K3D12::NonRaiiManagerComponentBase<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		//_resourceMap[name];
		_resourceMap[name] = resource;

	}
}

template<class T>
inline T* K3D12::NonRaiiManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return &_resourceMap[name];
	}
	return nullptr;
}

template<class T>
inline void K3D12::NonRaiiManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D12::NonRaiiManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();

}

template<class T>
inline bool K3D12::NonRaiiManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::map<std::string, T>& K3D12::NonRaiiManagerComponentBase<T>::GetMap()
{
	return _resourceMap;
}

//UnorderedMap

template<class T>
inline void K3D12::UnorderedManagerComponentBase<T>::Set(std::string name, std::shared_ptr<T> resource, bool forceOverride)
{

	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = resource;

	}
}

template<class T>
inline void K3D12::UnorderedManagerComponentBase<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::make_shared<T>(resource);
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = std::make_shared<T>(resource);

	}
}

template<class T>
inline std::weak_ptr<T> K3D12::UnorderedManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return _resourceMap[name];
	}
	return std::weak_ptr<T>();
}

template<class T>
inline void K3D12::UnorderedManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D12::UnorderedManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();
}  

template<class T>
inline bool K3D12::UnorderedManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::unordered_map<std::string, std::shared_ptr<T>>& K3D12::UnorderedManagerComponentBase<T>::GetMap()
{
	return _resourceMap;
}


