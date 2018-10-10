#include "RootRenderingPath.h"
#include "RenderingPathBase.h"
#include "../CommandContext/GraphicsContextLibrary.h"

K3D12::RootRenderingPath::RootRenderingPath(CommandQueue & excutionQueueRef) :
	_excutionQueue(excutionQueueRef) , _isEnable(true)
{
}

K3D12::RootRenderingPath::RootRenderingPath(const RootRenderingPath & other) :
	_excutionQueue(other._excutionQueue), _isEnable(other._isEnable)

{
}

K3D12::RootRenderingPath::RootRenderingPath(RootRenderingPath && other) :
	_excutionQueue(other._excutionQueue), _isEnable(other._isEnable)
{
}

K3D12::RootRenderingPath & K3D12::RootRenderingPath::operator=(const RootRenderingPath & other)
{

	return *this;
}

K3D12::RootRenderingPath & K3D12::RootRenderingPath::operator=(RootRenderingPath && other)
{
	return *this;
}


K3D12::RootRenderingPath::~RootRenderingPath()
{
	DiscardRenderingPath();
}


void K3D12::RootRenderingPath::Rendering()
{

	std::vector<ID3D12CommandList*> lists;
	for (auto& list : this->_renderingPathMap.GetMap()) {

		if (list.second->IsEnable()) {
			lists.push_back(GraphicsContextLibrary::GetInstance().GetGraphicsCommandList(list.first)->GetCommandList().Get());
		}
	}
	_excutionQueue.GetQueue()->ExecuteCommandLists(static_cast<UINT>(lists.size()), &lists[0]);

	_excutionQueue.Wait();
}

void K3D12::RootRenderingPath::ChangeExcutionQueue(CommandQueue & excutionQueue)
{
	_excutionQueue = excutionQueue;
}

std::shared_ptr<K3D12::RenderingPathBase> K3D12::RootRenderingPath::AddRenderingPath(std::string name, bool isEnable)
{
	if (!this->_renderingPathMap.IsLooded(name)) {
		//本体作成
		auto path = std::make_shared<RenderingPathBase>();
		//レンダリングパス配列にセット
		this->_renderingPathMap.Set(name, path);
		//名前からコマンドリストを作成
		GraphicsContextLibrary::GetInstance().CreateCommandList(name, 0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		//パスにセット
		path->SetCommandList(GraphicsContextLibrary::GetInstance().GetGraphicsCommandList(name));
		return path;
	}
	return nullptr;
}

void K3D12::RootRenderingPath::EraseRenderingPath(std::string name)
{
	if (this->_renderingPathMap.IsLooded(name)) {
		this->_renderingPathMap.Erase(name);
		GraphicsContextLibrary::GetInstance().EraseCommandList(name);
	}
}

std::weak_ptr<K3D12::RenderingPathBase> K3D12::RootRenderingPath::GetRenderingPath(std::string name)
{
	if (this->_renderingPathMap.IsLooded(name)) {
		return this->_renderingPathMap.Get(name);
	}
	return std::weak_ptr<RenderingPathBase>();
}

bool K3D12::RootRenderingPath::IsLoaded(std::string name)
{
	return this->_renderingPathMap.IsLooded(name);
}

void K3D12::RootRenderingPath::DiscardRenderingPath()
{
	this->_renderingPathMap.DiscardMap();
}

void K3D12::RootRenderingPath::Enable()
{
	this->_isEnable = true;
}

void K3D12::RootRenderingPath::Disable()
{
	this->_isEnable = false;
}

bool K3D12::RootRenderingPath::IsEnable()
{
	return this->_isEnable;
}


