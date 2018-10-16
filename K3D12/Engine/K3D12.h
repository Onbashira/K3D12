#pragma once

#ifndef __K3D12_H__

#define __K3D12_H__
//D3D12

//Game
#include "Animator/Animator.h"
#include "Animator/MotionAssetPool.h"
#include "Animator/MotionManager.h"
#include "Animator/StateMachineController.h"
#include "Animator/StateNode.h"
#include "Animator/TransitionConditions.h"
#include "Animator/VMD.h"
#include "Animator/VMDLoader.h"

#include "AsyncComponent/ThreadPool.h"

#include "Audio/Audio.h"
#include "Audio/AudioCallBack.h"
#include "Audio/AudioLoader.h"
#include "Audio/AudioManager.h"
#include "Audio/AudioNormalWav.h"
#include "Audio/AudioSourceManager.h"
#include "Audio/IWaveResource.h"
#include "Audio/SubMixVoice.h"
#include "Audio/SubMixVoiceManager.h"

#include "DrawnableComponent/DrawableComponent.h"
#include "Font/FontData.h"
#include "Font/FontsManager.h"
#include "GameConponent/Camera/Camera.h"
#include "GameConponent/GameObject/GameObject.h"
#include "GameConponent/Light/Light.h"
#include "GameConponent/Light/LightManager.h"
#include "GameConponent/Primitive/PrimitiveCreater.h"
#include "GameConponent/Primitive/Cube.h"
#include "GameConponent/Primitive/Plane.h"

#include "Input/GamePad.h"
#include "Input/GamePadManager.h"
#include "Input/InputManager.h"

#include "Mesh/MeshBuffer.h"
#include "Mesh/MeshHeap.h"
#include "Mesh/ModelMesh.h"
#include "Mesh/SkinedMeshComponent.h"
#include "Mesh/StaticMeshComponent.h"

#include "Model/MMDModel/MMDModel.h"
#include "Model/ModelObject.h"
#include "Model/ModelPool.h"
#include "Model/MMDModel/MMDModelManager.h"
#include "Model/MMDModel/MMDModelResourceData.h"
#include "Model/MMDModel/MMDModelResourceDataManager.h"

#include "Rendering/GBufferSprite.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/RenderingPathBase.h"
#include "Rendering/RenderingPathManager.h"
#include "Rendering/RootRenderingPath.h"
#include "Rendering/SceneRenderer.h"

#include "ShaderComponent/HLSLIncluder.h"
#include "ShaderComponent/ShaderCluster.h"
#include "ShaderComponent/ShaderManager.h"
#include "ShaderComponent/ShaderObjectLibrary.h"

#include "TextureComponent/TextureLoader.h"
#include "TextureComponent/TextureManager.h"

#include "Util/Logger.h"
#include "Util/ManagerComponentBase.h"
#include "Util/Math.h"
#include "Util/NonCopyable.h"
#include "Util/Utility.h"
#include "Util/WICTextureLoader12.h"


#endif

//Util