#include "BoxMesh.h"

UBoxMesh::UBoxMesh()
{

}

UBoxMesh::~UBoxMesh()
{

}

void UBoxMesh::Init()
{
	Super::Init();
}

void UBoxMesh::Draw(float InDeltaTime)
{
	Super::Draw(InDeltaTime);
}

void UBoxMesh::BuildMesh(const FMeshRenderingData* InData)
{
	Super::BuildMesh(InData);
}

UBoxMesh* UBoxMesh::CreateMesh()
{
	FMeshRenderingData RenderingData;
	/**
	 *	    7___________6 
	 *	  / |		 / |
	 *  /	|	   /   |
	 * 4-----------5-  |
	 * |	|3____|___2/
	 * |   /	  |  /
	 * |  /		  | /
	 * 0-----------1
	 */
	std::vector<FVertexData> pArr;//0~3底部的四个点 4~7顶部的四个点
	pArr.push_back(FVertexData(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT4(Colors::White)));
	pArr.push_back(FVertexData(XMFLOAT3(0.f, 2.f, 0.f), XMFLOAT4(Colors::AliceBlue)));
	pArr.push_back(FVertexData(XMFLOAT3(2.f, 2.f, 0.f), XMFLOAT4(Colors::Aqua)));
	pArr.push_back(FVertexData(XMFLOAT3(2.f, 0.f, 0.f), XMFLOAT4(Colors::Aquamarine)));
	pArr.push_back(FVertexData(XMFLOAT3(0.f, 0.f, 2.f), XMFLOAT4(Colors::Bisque)));
	pArr.push_back(FVertexData(XMFLOAT3(0.f, 2.f, 2.f), XMFLOAT4(Colors::Blue)));
	pArr.push_back(FVertexData(XMFLOAT3(2.f, 2.f, 2.f), XMFLOAT4(Colors::Chocolate)));
	pArr.push_back(FVertexData(XMFLOAT3(2.f, 0.f, 2.f), XMFLOAT4(Colors::Chocolate)));

	std::vector<uint16_t> IndexData;
	//前
	IndexData.push_back(0); IndexData.push_back(1); IndexData.push_back(2);
	IndexData.push_back(0); IndexData.push_back(2); IndexData.push_back(3);

	//后
	IndexData.push_back(4); IndexData.push_back(6); IndexData.push_back(5);
	IndexData.push_back(4); IndexData.push_back(7); IndexData.push_back(6);

	//左
	IndexData.push_back(4); IndexData.push_back(5); IndexData.push_back(1);
	IndexData.push_back(4); IndexData.push_back(1); IndexData.push_back(0);

	//右
	IndexData.push_back(3); IndexData.push_back(2); IndexData.push_back(6);
	IndexData.push_back(3); IndexData.push_back(6); IndexData.push_back(7);

	//上
	IndexData.push_back(1); IndexData.push_back(5); IndexData.push_back(6);
	IndexData.push_back(1); IndexData.push_back(6); IndexData.push_back(2);

	//下
	IndexData.push_back(4); IndexData.push_back(0); IndexData.push_back(3);
	IndexData.push_back(4); IndexData.push_back(3); IndexData.push_back(7);

	RenderingData.VertexData= std::move(pArr);
	RenderingData.IndexData = std::move(IndexData);

	//std::vector<FVertexData> vertices =
	//{
	//	FVertexData({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White)     }),
	//	FVertexData({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black)     }),
	//	FVertexData({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red)       }),
	//	FVertexData({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green)     }),
	//	FVertexData({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue)      }),
	//	FVertexData({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow)    }),
	//	FVertexData({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan)      }),
	//	FVertexData({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta)   })
	//};

	//std::vector<std::uint16_t> indices =
	//{
	//	// front face
	//	0, 1, 2,
	//	0, 2, 3,

	//	// back face
	//	4, 6, 5,
	//	4, 7, 6,

	//	// left face
	//	4, 5, 1,
	//	4, 1, 0,

	//	// right face
	//	3, 2, 6,
	//	3, 6, 7,

	//	// top face
	//	1, 5, 6,
	//	1, 6, 2,

	//	// bottom face
	//	4, 0, 3,
	//	4, 3, 7
	//};
	//RenderingData.VertexData= std::move(vertices);
 //   RenderingData.IndexData = std::move(indices);
	UBoxMesh* BoxMesh=new UBoxMesh;
	BoxMesh->BuildMesh(&RenderingData);
	BoxMesh->Init();
	return BoxMesh;
}

