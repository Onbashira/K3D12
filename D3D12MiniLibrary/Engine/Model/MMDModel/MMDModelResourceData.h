#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../../Util/Math.h"
#include "../PMX/PMXModel.h"
#include "../PMD/PMDModel.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../DescriptorHeap/DescriptorHeap.h"


namespace K3D12 {

	class ModelConverter;

	enum class MMDWeightDeformType {
		BDEF1,
		BDEF2,
		BDEF4,
		SDEF,
		QDEF
	};

	enum class MMDMaterialFlags {
		NO_CULL,
		GROUND_SHADOW,
		DRAW_SHADOW,
		RECEIVE_SHADOW,
		HAS_EDGE,
		VERTEX_COLOR,
		POINT_DRAWING,
		LINE_DRAWING
	};

	enum class MMDBoneFlags {
		INDEXED_TAIL_POSITION,
		ROTATABLE,
		TRANSLATABLE,
		IS_VISIBLE,
		ENABLE,
		IK,
		INHERIT_ROTATION,
		INHERIT_TRANSLATION,
		FIXED_AXIS,
		LOCAL_COORDINATE,
		PHISICS_AFTER_DEFORM,
		EXTERNAL_PARENT_DEFORM

	};

	enum class MMDSphereBlendType {
		NONE,
		MULT,
		ADD,
		SUB_TEXURE
	};


	//オリジナルデータを編集（変換）して、その情報の必要部分をこの構造体に代入　→　めっちゃおもそう　→　非同期ならどうだろうか
	//レンダリングに必要な頂点情報(変換済み頂点情報）
	struct MMDVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
		MMDWeightDeformType deformType;
		WeightDeform deformation;
		MMDVertex() : pos(), normal(), texCoord(), deformType(), deformation() {};
		~MMDVertex() {};
		MMDVertex(const MMDVertex& other) {
			*this = other;
		}
		MMDVertex& operator= (const MMDVertex& rhs) {
			pos = rhs.pos;
			normal = rhs.normal;
			texCoord = rhs.texCoord;
			deformType = rhs.deformType;
			deformation = rhs.deformation;
			return *this;
		}
	};

	//レンダリングを補助するマテリアル情報
	struct MMDMaterial {
		//	拡散反射光 w = alpha
		Vector4 diffuse;
		//　鏡面反射光	w = pow
		Vector4 specular;
		//環境光
		Vector3 ambient;
		//自己発光色
		Vector3 emissive;
		//スフィアマップのブレンディングタイプ
		int	sphereBlendType;
		//テクスチャテーブルのインデックス
		unsigned int textureTableIndex;
		//スフィアマップのインデックス
		unsigned int	sphereIndex;
		//LUTの参照テーブル番号
		unsigned int	toonIndex;
		//次のマテリアル描画までのインデックスオフセット
		unsigned int	surfaceCount;
	};

	//レンダリング時にセットするインデックス情報
	struct MMDIndexList {
		std::vector<unsigned int> list;
	};

	//レンダリング時に必要なテクスチャ情報
	struct MMDTextureHeap {
		std::weak_ptr<DescriptorHeap> texturesHeap;
	};

	struct MMDBoneNode {
		//親へのインデックス
		unsigned int parentIndex;
		//自身のインデックス
		unsigned int index;
		//ボーンの位置
		Vector3	pos;
		//初期ポーズからの回転量（四元数）
		Quaternion	rotation;
		unsigned short	boneOptionFlags;
		struct MMDBoneOption {
			//継承ボーン
			InheritBone inheritBone;
			//ローカル軸
			BoneFixedAxis fixidAxis;
			//固定軸
			BoneLocalCoordinate localAxis;
			//外部親インデックス
			BoneExternalParent	externalParentBoneIndex;
		}boneOption;
		//このボーンを親として参照するボーン
		std::vector<MMDBoneNode> childrenBone;
		void Discard() {
			for (auto& child : childrenBone) {
				child.Discard();
			}
			this->childrenBone.clear();
		};

		MMDBoneNode() {};
		~MMDBoneNode() {
		};
	};




	struct MMDIKLinkData {
		unsigned int boneIndex;
		bool hasLimit;		//下限値上限値の判断フラグ
		IKAngleLimit limits;
	};

	struct MMDIKData {
		unsigned int boneIndex;
		unsigned int targetBoneIndex; //IKボーンが最初にコネクティングするボーンインデックス
		int loopCount;
		float limitRadian;
		std::vector<MMDIKLinkData> ikLinks;
		MMDIKData() {};
		~MMDIKData() {
			ikLinks.clear();
		}
	};

	struct MMDBoneTree {
		unsigned int boneNum;
		std::map<std::string, MMDBoneNode*>					boneAccessor;
		std::map<unsigned int, std::string>					boneNameAccessor;
		std::vector<Matrix>									bonesMatrix;
		std::vector<MMDIKData>								modelIKdata;
		MMDBoneNode											rootBone;
		MMDBoneTree() {

		};

		~MMDBoneTree() {
			bonesMatrix.clear();
			modelIKdata.clear();
			boneAccessor.clear();
		};
	};

	//MMDで読み込めるモデルデータ（PMX・PMD）のオリジナルデータ
	struct MMDModelResourceData
	{
		//頂点情報
		std::vector<MMDVertex>						_vertexes;
		//頂点インデックス情報
		MMDIndexList								_indexList;
		//テクスチャ情報
		MMDTextureHeap								_textureHeaps;
		//マテリアル情報
		std::vector<MMDMaterial>					_materials;
		//スケルトン
		std::shared_ptr<MMDBoneTree>				_boneTree;
		//テクスチャパス
		std::vector<std::string>					_texturePaths;
		//モデルパス
		std::string									_modelPath;
		MMDModelResourceData();
		~MMDModelResourceData();
	};
}