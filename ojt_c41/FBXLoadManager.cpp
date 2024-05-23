#include "pch.h"
#include "framework.h"
#include "FBXLoadManager.h"

//#include "Helper\Memory.h",
FBXLoadManager::FBXLoadManager()
	: mFbxManager(nullptr)
	, mIos(nullptr)
{
	mFbxManager = FbxManager::Create();
	Assert(mFbxManager, ASSERT_MSG_NULL);

	mIos = FbxIOSettings::Create(mFbxManager, IOSROOT);
	Assert(mIos, ASSERT_MSG_NULL);
}

FBXLoadManager::~FBXLoadManager()
{
	mIos->Destroy();

	mFbxManager->Destroy();
}

void FBXLoadManager::Load(const std::wstring& wFilePath)
{
	const std::string& FILE_PATH = StringHelper::WStrToStr(wFilePath);
	FbxImporter* const imposter = FbxImporter::Create(mFbxManager, "");
	Assert(imposter, ASSERT_MSG_NULL);

	bool bImportStatus = imposter->Initialize(FILE_PATH.c_str(), -1, mFbxManager->GetIOSettings());
	Assert(bImportStatus, ASSERT_MSG_INVALID);

	FbxScene* const fbxScene = FbxScene::Create(mFbxManager, "");
	Assert(fbxScene, ASSERT_MSG_NULL);
	imposter->Import(fbxScene);
	fbxScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	mVecContainer.clear();

	loadSkeleton(fbxScene->GetRootNode());
	fbxScene->FillAnimStackNameArray(m_arrAnimName);
	loadAnimationClip(fbxScene);

	//FbxPose* pose = 

	triangulate(fbxScene->GetRootNode());

	loadMeshDataFromNode(fbxScene, fbxScene->GetRootNode());

	imposter->Destroy();
	fbxScene->Destroy();
}

void FBXLoadManager::Release()
{
	mem::del::DeleteVectorElements(&m_vecAnimClip);

	for (int i = 0; i < m_arrAnimName.GetCount(); ++i)
	{
		FbxString* takeName = m_arrAnimName.GetAt(i);
		DELETE_POINTER(takeName);
	}
	m_arrAnimName.Clear();
}

void FBXLoadManager::loadMeshDataFromNode(FbxScene* const fbxScene, FbxNode* fbxNode)
{
	// 노드의 메쉬정보 읽기
	FbxNodeAttribute* attribute = fbxNode->GetNodeAttribute();
	if (attribute)
	{
		FbxNodeAttribute::EType ATTRIBUTE_TYPE = attribute->GetAttributeType();
		if (FbxNodeAttribute::eMesh == ATTRIBUTE_TYPE)
		{
			FbxAMatrix matGlobal = fbxNode->EvaluateGlobalTransform();
			matGlobal.GetR();

			FbxMesh* fbxMesh = fbxNode->GetMesh();			
			lodeMesh(fbxScene, fbxMesh);
		}
	}

	// 해당 노드의 재질정보 읽기
	const int MATERIAL_COUNT = fbxNode->GetMaterialCount();
	for (int i = 0; i < MATERIAL_COUNT; ++i)
	{
		FbxSurfaceMaterial* pMtrlSur = fbxNode->GetMaterial(i);		
		if (!mVecContainer.empty()) 
		{
			lodeMaterial(pMtrlSur);
		}			
	}

	// 자식 노드 정보 읽기
	const int CHILD_NODE_COUNT = fbxNode->GetChildCount();
	for (int i = 0; i < CHILD_NODE_COUNT; ++i)
	{
		loadMeshDataFromNode(fbxScene, fbxNode->GetChild(i));
	}
}

void FBXLoadManager::lodeMesh(FbxScene* const fbxScene, FbxMesh* FbxMesh)
{	
	mVecContainer.push_back(tContainer{});
	tContainer& container = mVecContainer[mVecContainer.size() - 1];
	const std::string& MESH_NAME = FbxMesh->GetName();

	container.strName = StringHelper::StrToWStr(MESH_NAME);
	container.vecBone = m_vecOffsetBone;
	const int VERTEX_COUNT = FbxMesh->GetControlPointsCount();
	container.Resize(VERTEX_COUNT);
	const FbxVector4* const P_VERTEX_POS_ARRAY = FbxMesh->GetControlPoints();
	for (int i = 0; i < VERTEX_COUNT; ++i)
	{
		container.vecPos[i].x = static_cast<float>(P_VERTEX_POS_ARRAY[i].mData[0]);
		container.vecPos[i].y = static_cast<float>(P_VERTEX_POS_ARRAY[i].mData[2]);
		container.vecPos[i].z = static_cast<float>(P_VERTEX_POS_ARRAY[i].mData[1]);
	}

	// 재질의 개수 ( ==> SubSet 개수 ==> Index Buffer Count)
	const int MATERIAL_COUNT = FbxMesh->GetNode()->GetMaterialCount();
	container.vecIdx.resize(MATERIAL_COUNT);

	// 정점 정보가 속한 subset 을 알기위해서...
	FbxGeometryElementMaterial* GEMaterial = FbxMesh->GetElementMaterial();

	// 폴리곤을 구성하는 정점 개수
	const int POLY_SIZE = FbxMesh->GetPolygonSize(0);
	if (3 != POLY_SIZE)
	{		
		Assert(false, ASSERT_MSG_INVALID);
	}

	UINT poly[3] = { 0, };
	UINT IndexIdx = 0; // 폴리곤 순서로 접근하는 순번
	const int POLY_COUNT = FbxMesh->GetPolygonCount();
	for (int i = 0; i < POLY_COUNT; ++i)
	{
		for (int j = 0; j < POLY_SIZE; ++j)
		{
			// i 번째 폴리곤에, j 번째 정점
			const int VERTEX_IDX = FbxMesh->GetPolygonVertex(i, j);
			poly[j] = VERTEX_IDX;

			getTangent(FbxMesh, &container, VERTEX_IDX, IndexIdx);
			getBinormal(FbxMesh, &container, VERTEX_IDX, IndexIdx);
			getNormal(FbxMesh, &container, VERTEX_IDX, IndexIdx);
			getUV(FbxMesh, &container, VERTEX_IDX, FbxMesh->GetTextureUVIndex(i, j));

			++IndexIdx;
		}

		const int SUB_SUB_IDX = GEMaterial->GetIndexArray().GetAt(i);
		container.vecIdx[SUB_SUB_IDX].push_back(poly[0]);
		container.vecIdx[SUB_SUB_IDX].push_back(poly[2]);
		container.vecIdx[SUB_SUB_IDX].push_back(poly[1]);
	}
	loadAnimationData(fbxScene, FbxMesh, &container);
}

void FBXLoadManager::triangulate(FbxNode* _pNode)
{
	FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

	if (pAttr &&
		(pAttr->GetAttributeType() == FbxNodeAttribute::eMesh
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs
			|| pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter converter(mFbxManager);
		converter.Triangulate(pAttr, true);
	}

	int iChildCount = _pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
	{
		triangulate(_pNode->GetChild(i));
	}
}

Vector4 FBXLoadManager::GetMtrlData(FbxSurfaceMaterial* _pSurface,
	const char* _pMtrlName,
	const char* _pMtrlFactorName)
{
	FbxDouble3  vMtrl;
	FbxDouble	dFactor = 0.;

	FbxProperty tMtrlProperty = _pSurface->FindProperty(_pMtrlName);
	FbxProperty tMtrlFactorProperty = _pSurface->FindProperty(_pMtrlFactorName);

	if (tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid())
	{
		vMtrl = tMtrlProperty.Get<FbxDouble3>();
		dFactor = tMtrlFactorProperty.Get<FbxDouble>();
	}

	Vector4 vRetVal = Vector4((float)vMtrl.mData[0] * (float)dFactor, (float)vMtrl.mData[1] * (float)dFactor, (float)vMtrl.mData[2] * (float)dFactor, (float)dFactor);
	return vRetVal;
}

std::wstring FBXLoadManager::GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty)
{
	std::string strName;

	FbxProperty TextureProperty = _pSurface->FindProperty(_pMtrlProperty);
	if (TextureProperty.IsValid())
	{
		UINT iCnt = TextureProperty.GetSrcObjectCount();

		if (1 <= iCnt)
		{
			FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<FbxFileTexture>(0);
			if (NULL != pFbxTex)
				strName = pFbxTex->GetFileName();
		}
	}

	return StringHelper::StrToWStr(strName);
}

void FBXLoadManager::lodeMaterial(FbxSurfaceMaterial* _pMtrlSur)
{
	tFbxMaterial tMtrlInfo{};

	std::string str = _pMtrlSur->GetName();
	tMtrlInfo.strMtrlName = StringHelper::StrToWStr(str);

	// Diff
	tMtrlInfo.tMtrl.vDiff = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sDiffuse
		, FbxSurfaceMaterial::sDiffuseFactor);

	// Amb
	tMtrlInfo.tMtrl.vAmb = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sAmbient
		, FbxSurfaceMaterial::sAmbientFactor);

	// Spec
	tMtrlInfo.tMtrl.vSpec = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sSpecular
		, FbxSurfaceMaterial::sSpecularFactor);

	// Emisv
	tMtrlInfo.tMtrl.vEmv = GetMtrlData(_pMtrlSur
		, FbxSurfaceMaterial::sEmissive
		, FbxSurfaceMaterial::sEmissiveFactor);

	// Texture Name
	tMtrlInfo.strDiff = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sDiffuse);
	tMtrlInfo.strNormal = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sNormalMap);
	tMtrlInfo.strSpec = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sSpecular);
	tMtrlInfo.strEmis = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sEmissive);


	mVecContainer.back().vecMtrl.push_back(tMtrlInfo);
}

void FBXLoadManager::loadTextrue()
{
	for (UINT i = 0; i < mVecContainer.size(); ++i)
	{

		for (UINT j = 0; j < mVecContainer[i].vecMtrl.size(); ++j)
		{
			std::vector<std::wstring> vecPath;
			vecPath.push_back(mVecContainer[i].vecMtrl[j].strDiff.c_str());
			vecPath.push_back(mVecContainer[i].vecMtrl[j].strNormal.c_str());
			vecPath.push_back(mVecContainer[i].vecMtrl[j].strSpec.c_str());
			vecPath.push_back(mVecContainer[i].vecMtrl[j].strEmis.c_str());

			for (std::wstring path : vecPath)
			{

			}
		}
	}
}

void FBXLoadManager::loadSkeleton(FbxNode* rootNode)
{
	const int CHILD_COUNT = rootNode->GetChildCount();
	for (int i = 0; i < CHILD_COUNT; ++i)
	{
		FbxNode* currNode = rootNode->GetChild(i);
		loadSkeletonRe(currNode, 0, 0, -1);
	}
}

void FBXLoadManager::loadSkeletonRe(FbxNode* _pNode, int depth, int Idx, int parentIdx)
{
	FbxNodeAttribute* const attr = _pNode->GetNodeAttribute();
	if (attr)
	{
		const FbxNodeAttribute::EType AT_TYPE = attr->GetAttributeType();
		if (AT_TYPE == FbxNodeAttribute::eSkeleton)
		{
			tBone bone = {};
			const std::string& BONE_NAME = _pNode->GetName();
			bone.boneName = StringHelper::StrToWStr(BONE_NAME);
			bone.depth = depth++;
			bone.parentIdx = parentIdx;
			m_vecOffsetBone.push_back(bone);			
		}
	}

	const int CHILD_COUNT = _pNode->GetChildCount();
	for (int i = 0; i < CHILD_COUNT; ++i)
	{
		loadSkeletonRe(_pNode->GetChild(i), depth, (int)m_vecOffsetBone.size(), Idx);
	}
}

void FBXLoadManager::loadAnimationClip(FbxScene* const fbxScene)
{
	int iAnimCount = m_arrAnimName.GetCount();

	for (int i = 0; i < iAnimCount; ++i)
	{
		FbxAnimStack* pAnimStack = fbxScene->FindMember<FbxAnimStack>(m_arrAnimName[i]->Buffer());


		//FbxAnimEvaluator* pevaluator = m_pScene->GetAnimationEvaluator();
		//m_pScene->SetCurrentAnimationStack();


		if (!pAnimStack)
		{
			continue;
		}

		tAnimClip* pAnimClip = new tAnimClip;

		std::string strClipName = pAnimStack->GetName();
		pAnimClip->strName = StringHelper::StrToWStr(strClipName);

		FbxTakeInfo* pTakeInfo = fbxScene->GetTakeInfo(pAnimStack->GetName());
		pAnimClip->tStartTime = pTakeInfo->mLocalTimeSpan.GetStart();
		pAnimClip->tEndTime = pTakeInfo->mLocalTimeSpan.GetStop();

		pAnimClip->eMode = fbxScene->GetGlobalSettings().GetTimeMode();
		pAnimClip->llTimeLength = pAnimClip->tEndTime.GetFrameCount(pAnimClip->eMode) -
			pAnimClip->tStartTime.GetFrameCount(pAnimClip->eMode);



		m_vecAnimClip.push_back(pAnimClip);
	}
}

void FBXLoadManager::loadAnimationData(FbxScene* const fbxScene, FbxMesh* _pMesh, tContainer* _pContainer)
{
	// Animation Data 로드할 필요가 없음
	const int SKIN_COUNT = _pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (SKIN_COUNT <= 0)
	{
		return;
	}
	_pContainer->bAnimation = true;
	FbxAMatrix geometryTransform = getGeometryTransformation(_pMesh->GetNode());


	// Skin 개수만큼 반복을하며 읽는다.
	// 바인드포즈는 메쉬마다 다르다.
	// 보통 메쉬당 Deformer는 하나이다.
	// cluster 가 joint 라고 생각할수있으나 진짜 joint 는 cluster에서 Link 를통해 정보들을 얻어온다.
	for (int i = 0; i < SKIN_COUNT; ++i)
	{
		FbxSkin* const fbxSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);
		Assert(fbxSkin, ASSERT_MSG_NULL);
		const FbxSkin::EType FBX_SKIN_TYPE = fbxSkin->GetSkinningType();
		if (FbxSkin::eRigid == FBX_SKIN_TYPE || FbxSkin::eLinear == FBX_SKIN_TYPE)
		{
			// Cluster 를 얻어온다 Cluster == Joint == 관절			
			const int CLUSTER_COUNT = fbxSkin->GetClusterCount();
			for (int j = 0; j < CLUSTER_COUNT; ++j)
			{
				FbxCluster* const fbxCluster = fbxSkin->GetCluster(j);				
				if (nullptr == fbxCluster) //클러스터가없는경우
				{
					continue;
				}
				const std::string& BONE_NAME = fbxCluster->GetLink()->GetName();
				const int BOND_IDX = findBoneIndex(BONE_NAME, _pContainer);
				Assert(-1 != BOND_IDX, ASSERT_MSG_INVALID);
				FbxAMatrix matrixNodeTransform = getGeometryTransformation(_pMesh->GetNode());

				// Weights And Indices 정보를 읽는다.
				loadWeightsAndIndices(fbxCluster, BOND_IDX, _pContainer);

				// Bone 의 OffSet 행렬 구한다.
				loadOffsetMatrix(fbxCluster, matrixNodeTransform, BOND_IDX, _pContainer);

				// Bone KeyFrame 별 행렬을 구한다.
				loadKeyframeTransform(fbxScene, _pMesh->GetNode(),
					fbxCluster, matrixNodeTransform, BOND_IDX, _pContainer);
			}
		}
	}
	checkWeightAndIndices(_pContainer);
}

int FBXLoadManager::findBoneIndex(const std::string& _strBoneName, tContainer* _pContainer)
{
	std::wstring strBoneName = StringHelper::StrToWStr(_strBoneName);

	for (UINT i = 0; i < _pContainer->vecBone.size(); ++i)
	{
		if (_pContainer->vecBone[i].boneName == strBoneName)
		{
			return i;
		}
	}

	return -1;
}

void FBXLoadManager::loadWeightsAndIndices(const FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer)
{
	int iIndicesCount = _pCluster->GetControlPointIndicesCount();

	for (int i = 0; i < iIndicesCount; ++i)
	{
		tWeightsAndIndices tWI = {};

		// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
		tWI.iBoneIdx = _iBoneIdx;
		tWI.dWeight = _pCluster->GetControlPointWeights()[i];

		int iVtxIdx = _pCluster->GetControlPointIndices()[i];

		_pContainer->vecWI[iVtxIdx].push_back(tWI);
	}
}

void FBXLoadManager::loadOffsetMatrix(FbxCluster* _pCluster,
	const FbxAMatrix& _matNodeTransform,
	int _iBoneIdx,
	tContainer* _pContainer)
{
	FbxAMatrix matClusterTrans;
	FbxAMatrix matClusterLinkTrans;

	_pCluster->GetTransformMatrix(matClusterTrans);
	_pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

	// Reflect Matrix
	FbxVector4 V0 = { 1, 0, 0, 0 };
	FbxVector4 V1 = { 0, 0, 1, 0 };
	FbxVector4 V2 = { 0, 1, 0, 0 };
	FbxVector4 V3 = { 0, 0, 0, 1 };

	FbxAMatrix matReflect;
	matReflect[0] = V0;
	matReflect[1] = V1;
	matReflect[2] = V2;
	matReflect[3] = V3;

	FbxAMatrix matOffset;
	matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * _matNodeTransform;
	matOffset = matReflect * matOffset * matReflect;

	_pContainer->vecBone[_iBoneIdx].matOffset = matOffset;
}

void FBXLoadManager::loadKeyframeTransform(FbxScene* const fbxScene,
	FbxNode* _pNode,
	FbxCluster* _pCluster,
	const FbxAMatrix& _matNodeTransform,
	int _iBoneIdx,
	tContainer* _pContainer)
{
	if (m_vecAnimClip.empty())
		return;

	FbxVector4	v1 = { 1, 0, 0, 0 };
	FbxVector4	v2 = { 0, 0, 1, 0 };
	FbxVector4	v3 = { 0, 1, 0, 0 };
	FbxVector4	v4 = { 0, 0, 0, 1 };
	FbxAMatrix	matReflect;
	matReflect.mData[0] = v1;
	matReflect.mData[1] = v2;
	matReflect.mData[2] = v3;
	matReflect.mData[3] = v4;

	_pContainer->vecBone[_iBoneIdx].matBone = _matNodeTransform;

	FbxTime::EMode eTimeMode = fbxScene->GetGlobalSettings().GetTimeMode();


	// 모든 AnimClip 의 행렬정보를 연산해서 하나의 프레임행렬 데이터 만들기
	for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
	{
		// 애님 클립 이름으로 FbxAnimStack 정보를 찾는다.
		std::string strClipName = StringHelper::WStrToStr(m_vecAnimClip[i]->strName);
		FbxAnimStack* pAnimStack = fbxScene->FindMember<FbxAnimStack>(strClipName.c_str());

		// 현재 AnimStack 을 변경한다.
		fbxScene->SetCurrentAnimationStack(pAnimStack);

		// 현재 Animstiack 의 행렬정보를 연산해서 Bone 에 프레임별 행렬정보를 채운다.
		FbxLongLong llStartFrame = m_vecAnimClip[i]->tStartTime.GetFrameCount(eTimeMode);
		FbxLongLong llEndFrame = m_vecAnimClip[i]->tEndTime.GetFrameCount(eTimeMode);

		for (FbxLongLong j = llStartFrame; j < llEndFrame; ++j)
		{
			tKeyFrame tFrame = {};
			FbxTime   tTime = 0;

			tTime.SetFrame(j, eTimeMode);

			FbxAMatrix matFromNode = _pNode->EvaluateGlobalTransform(tTime) * _matNodeTransform;
			FbxAMatrix matCurTrans = matFromNode.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(tTime);
			matCurTrans = matReflect * matCurTrans * matReflect;

			tFrame.dTime = tTime.GetSecondDouble();
			tFrame.matTransform = matCurTrans;

			_pContainer->vecBone[_iBoneIdx].vecKeyFrame.push_back(tFrame);
		}
	}
}

void FBXLoadManager::checkWeightAndIndices(tContainer* _pContainer)
{
	std::vector<std::vector<tWeightsAndIndices>>::iterator iter = _pContainer->vecWI.begin();

	int iVtxIdx = 0;
	for (iter; iter != _pContainer->vecWI.end(); ++iter, ++iVtxIdx)
	{
		if ((*iter).size() > 1)
		{
			// 가중치 값 순으로 내림차순 정렬
			std::sort((*iter).begin(), (*iter).end()
				, [](const tWeightsAndIndices& left, const tWeightsAndIndices& right)
				{
					return left.dWeight > right.dWeight;
				}
			);

			double dWeight = 0.f;
			for (UINT i = 0; i < (*iter).size(); ++i)
			{
				dWeight += (*iter)[i].dWeight;
			}

			// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
			double revision = 0.f;
			if (dWeight > 1.0)
			{
				revision = 1.0 - dWeight;
				(*iter)[0].dWeight += revision;
			}

			if ((*iter).size() >= 4)
			{
				(*iter).erase((*iter).begin() + 4, (*iter).end());
			}
		}

		// 정점 정보로 변환, 
		float fWeights[4] = {};
		float fIndices[4] = {};

		for (UINT i = 0; i < (*iter).size(); ++i)
		{
			fWeights[i] = (float)(*iter)[i].dWeight;
			fIndices[i] = (float)(*iter)[i].iBoneIdx;
		}

		memcpy(&_pContainer->vecWeights[iVtxIdx], fWeights, sizeof(Vector4));
		memcpy(&_pContainer->vecIndices[iVtxIdx], fIndices, sizeof(Vector4));
	}
}

FbxAMatrix FBXLoadManager::getGeometryTransformation(FbxNode* _pNode)
{
	const FbxVector4 T = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 R = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 S = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(T, R, S);
}

void FBXLoadManager::getTangent(FbxMesh* _pMesh
	, tContainer* _pContainer
	, int _iIdx		 /*해당 정점의 인덱스*/
	, int _iVtxOrder /*폴리곤 단위로 접근하는 순서*/)
{
	const int TANGENT_COUNT = _pMesh->GetElementTangentCount();

	if (1 > TANGENT_COUNT)
	{
		return;
	}

	if (1 < TANGENT_COUNT)
		assert(NULL); // 정점 1개가 포함하는 탄젠트 정보가 2개 이상이다.

	// 탄젠트 data 의 시작 주소
	FbxGeometryElementTangent* pTangent = _pMesh->GetElementTangent();
	UINT iTangentIdx = 0;

	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			iTangentIdx = _iVtxOrder;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
			iTangentIdx = _iIdx;
		else
			iTangentIdx = pTangent->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vTangent = pTangent->GetDirectArray().GetAt(iTangentIdx);

	_pContainer->vecTangent[_iIdx].x = (float)vTangent.mData[0];
	_pContainer->vecTangent[_iIdx].y = (float)vTangent.mData[2];
	_pContainer->vecTangent[_iIdx].z = (float)vTangent.mData[1];
}

void FBXLoadManager::getBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
	int iBinormalCnt = _pMesh->GetElementBinormalCount();
	if (1 > iBinormalCnt)
	{
		return;
	}

	if (1 != iBinormalCnt)
		assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	// 종법선 data 의 시작 주소
	FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();
	UINT iBinormalIdx = 0;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iBinormalIdx = _iVtxOrder;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iBinormalIdx = _iIdx;
		else
			iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);

	_pContainer->vecBinormal[_iIdx].x = (float)vBinormal.mData[0];
	_pContainer->vecBinormal[_iIdx].y = (float)vBinormal.mData[2];
	_pContainer->vecBinormal[_iIdx].z = (float)vBinormal.mData[1];
}

void FBXLoadManager::getNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
	int iNormalCnt = _pMesh->GetElementNormalCount();
	if (1 != iNormalCnt)
		assert(NULL); // 정점 1개가 포함하는 종법선 정보가 2개 이상이다.

	// 종법선 data 의 시작 주소
	FbxGeometryElementNormal* pNormal = _pMesh->GetElementNormal();
	UINT iNormalIdx = 0;

	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iVtxOrder;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iVtxOrder);
	}
	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
			iNormalIdx = _iIdx;
		else
			iNormalIdx = pNormal->GetIndexArray().GetAt(_iIdx);
	}

	FbxVector4 vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);

	_pContainer->vecNormal[_iIdx].x = (float)vNormal.mData[0];
	_pContainer->vecNormal[_iIdx].y = (float)vNormal.mData[2];
	_pContainer->vecNormal[_iIdx].z = (float)vNormal.mData[1];
}

void FBXLoadManager::getUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iUVIndex)
{
	FbxGeometryElementUV* pUV = _pMesh->GetElementUV();

	if (nullptr == pUV)
	{
		return;
	}

	UINT iUVIdx = 0;
	if (pUV->GetReferenceMode() == FbxGeometryElement::eDirect)
		iUVIdx = _iIdx;
	else
		iUVIdx = pUV->GetIndexArray().GetAt(_iIdx);

	iUVIdx = _iUVIndex;
	FbxVector2 vUV = pUV->GetDirectArray().GetAt(iUVIdx);
	_pContainer->vecUV[_iIdx].x = (float)vUV.mData[0];
	_pContainer->vecUV[_iIdx].y = 1.f - (float)vUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0
}
#pragma region  SecondTread

//DWORD SecondThread(PVOID pvParam)
//{
//	BYTE* pByte = reinterpret_cast<BYTE*>(pvParam);
//	FbxSkin* pSkin = (FbxSkin*)*(unsigned long long*)(pByte);
//	FbxScene* const fbxScene = (FbxScene*)*(unsigned long long*)(pByte + 8);
//	FbxMesh* _pMesh = (FbxMesh*)*(unsigned long long*)(pByte + 16);
//	tContainer* _pContainer = (tContainer*)*(unsigned long long*)(pByte + 24);
//	FBXLoadManager* manager = (FBXLoadManager*)*(unsigned long long*)(pByte + 32);
//	int idx = *(int*)(pByte + 40);
//
//
//	//FbxSkin* pSkin, FbxScene* const fbxScene, FbxMesh* _pMesh, tContainer* _pContainer, int idx
//	FbxCluster* pCluster = pSkin->GetCluster(idx);
//
//	if (!pCluster->GetLink())
//		return 0;
//
//	// 현재 본 인덱스를 얻어온다.
//	std::string boneName = pCluster->GetLink()->GetName();
//	int iBoneIdx = manager->findBoneIndex(boneName, _pContainer);
//	if (-1 == iBoneIdx)
//		assert(NULL);
//
//	FbxAMatrix matNodeTransform = manager->getTransform(_pMesh->GetNode());
//
//	// Weights And Indices 정보를 읽는다.
//	manager->loadWeightsAndIndices(pCluster, iBoneIdx, _pContainer);
//
//	// Bone 의 OffSet 행렬 구한다.
//	manager->loadOffsetMatrix(pCluster, matNodeTransform, iBoneIdx, _pContainer);
//
//	// Bone KeyFrame 별 행렬을 구한다.
//	manager->loadKeyframeTransform(fbxScene, _pMesh->GetNode(),
//		pCluster, matNodeTransform, iBoneIdx, _pContainer);
//	return 0;
//}

#pragma endregion