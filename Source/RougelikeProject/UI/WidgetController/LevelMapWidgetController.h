// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLWidgetController.h"
#include "RougelikeProject/Data/LevelConfig.h"
#include "LevelMapWidgetController.generated.h"

enum NodeType
{
	Start,
	Battle,
	Shop,
	End,
};

enum NodeState
{
	Lock,
	Incomplete,
	Complete,
	CompleteAndForbid,
};

enum LineState
{
	CanMove,
	Forbid
};

USTRUCT(Blueprintable)
struct FNode
{
	GENERATED_BODY()
	FNode(int _index, int _X, int _Y, FVector2D _pos, FVector2D _size, NodeType _type, NodeState _state, FName _levelName, int _arountCnt = 0)
	{
		NodeIndex = _index;
		bExist = true;
		point.X = _X;
		point.Y = _Y;
		position = _pos;
		size = _size;
		type = _type;
		state = _state;
		AroundCnt = _arountCnt;
		LevelName = _levelName;
	}

	FNode()
	{
		NodeIndex = -1;
		bExist = false;
		AroundCnt = 0;
	}

	UPROPERTY(BlueprintReadOnly)
	int NodeIndex;
	
	bool bExist;
	
	FVector2D point;
	FVector2D position;
	FVector2D size;

	NodeType type;
	NodeState state;

	int AroundCnt;

	FName LevelName;

	TArray<int> NeighborLines;
	TArray<int> NeighborNodes;
};

USTRUCT(Blueprintable)
struct FLine
{
	GENERATED_BODY()
	FLine(int index, FNode* n1, FNode* n2, int len)
	{
		LineIndex = index;
		node_1 = n1;
		node_2 = n2;

		lineLenth = len;
		state = Forbid;
	}

	FLine()
	{
		LineIndex = -1;
	}

	UPROPERTY(BlueprintReadOnly)
	int LineIndex;
	
	FNode* node_1;
	FNode* node_2;
	
	int lineLenth;
	
	LineState state;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateNodeSignature, FNode, node);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateLineSignature, FLine, line);

UCLASS(Blueprintable)
class ROUGELIKEPROJECT_API ULevelMapWidgetController : public URLWidgetController
{
	GENERATED_BODY()
public:
	ULevelMapWidgetController();

	UPROPERTY(BlueprintAssignable)
	FOnUpdateNodeSignature OnUpdateNode;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateLineSignature OnAddLines;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateLineSignature OnUpdateLines;
	
	UFUNCTION(BlueprintCallable)
	void GenerateLevelNode();

	UFUNCTION(BlueprintCallable)
	void GenerateLevelLine();
	
	UFUNCTION(BlueprintCallable)
	void UpdateLevel(int CurNodeIndex, int LastNodeIndex);
	
	UPROPERTY(EditAnywhere)
	FVector2D GridSize;

	UPROPERTY(EditAnywhere)
	int LineLenThreshold;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULevelConfig> LevelConfig;
	
protected:

	void CreateNode(TArray<TArray<FNode>>& NodeMap, FVector2D Size, FVector2D Position, int X, int Y, NodeType type, NodeState state, FName LevelName, int ArountCnt = 0);

	// 实际存储 node 和 line  // TODO：变成Map，点和线都用索引找，而不是实际矩阵位置
	
	TMap<int, FLine> LineData;
	TMap<int, FNode> NodeData;
	
	FVector2D StartPos;

	TMap<int, int> ColMap; // <列号， 已有节点数> // 保证中间三列每列一个，其余每列最多两个

	int LineCnt;
	int NodeCnt;

	float CrossProduct(FVector2D p1, FVector2D p2, FVector2D p3);
	bool IsOnSegment(FVector2D a, FVector2D b, FVector2D p);
	bool IsIntersecting(FVector2D p1, FVector2D p2, FVector2D p3, FVector2D p4);
};
