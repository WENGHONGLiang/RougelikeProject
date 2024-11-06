// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMapWidgetController.h"

#include <set>
#include <vector>

#include "Kismet/GameplayStatics.h"
#include "UniversalObjectLocators/UniversalObjectLocatorUtils.h"

ULevelMapWidgetController::ULevelMapWidgetController()
{

}

void ULevelMapWidgetController::GenerateLevelNode()
{
	NodeCnt = 0;
	
	TArray<TArray<FNode>> NodeMap;
	NodeMap.SetNum(15);
	for(int i = 0; i < 15; i++)
		NodeMap[i].SetNum(5);
	
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
	StartPos = ViewportCenter - FVector2D(15, 5) * GridSize;

	FLevelInfo StartLevelInfo = LevelConfig->GetLevelInfo(ELevelType::Level_Start);
	FLevelInfo BattleLevelInfo_1 = LevelConfig->GetLevelInfo(ELevelType::Level_Battle_1);
	FLevelInfo BattleLevelInfo_2 = LevelConfig->GetLevelInfo(ELevelType::Level_Battle_2);
	FLevelInfo BattleLevelInfo_3 = LevelConfig->GetLevelInfo(ELevelType::Level_Battle_3);
	FLevelInfo ShopLevelInfo_1 = LevelConfig->GetLevelInfo(ELevelType::Level_Shop_1);
	FLevelInfo ShopLevelInfo_2 = LevelConfig->GetLevelInfo(ELevelType::Level_Shop_2);
	FLevelInfo ShopLevelInfo_3 = LevelConfig->GetLevelInfo(ELevelType::Level_Shop_3);
	FLevelInfo BossLevelInfo = LevelConfig->GetLevelInfo(ELevelType::Level_Boss);
	
	// 起点
	int StartCol = FMath::RandRange(1, 3);
	int StartRow = FMath::RandRange(1, 3);
	FVector2D StartNodePosition = FVector2D(StartPos.X + GridSize.X * StartCol * 2, StartPos.Y + GridSize.Y * StartRow * 2);
	int StartLevelIndex = FMath::RandRange(0, StartLevelInfo.LevelMapName.Num()-1);
	FName StartLevelName = StartLevelInfo.LevelMapName[StartLevelIndex];
	CreateNode(NodeMap, FVector2d(2.5, 2.5), StartNodePosition, StartCol, StartRow, Start, Incomplete, StartLevelName);
	ColMap.Emplace(StartCol, 1);
	
	// 终点
	int EndCol = FMath::RandRange(11, 13);
	int EndRow = FMath::RandRange(1, 3);
	FVector2D EndNodePosition = FVector2D(StartPos.X + GridSize.X * EndCol * 2, StartPos.Y + GridSize.Y * EndRow * 2);
	int EndLevelIndex = FMath::RandRange(0, BossLevelInfo.LevelMapName.Num()-1);
	FName EndLevelName = BossLevelInfo.LevelMapName[EndLevelIndex];
	CreateNode(NodeMap,FVector2d(2.5, 2.5), EndNodePosition, EndCol, EndRow, End, Lock, EndLevelName);
	ColMap.Emplace(EndCol, 1);

	// 中间三列
	int MidCol_1 = FMath::RandRange(6, 7);
	int MidCol_2 = FMath::RandRange(7, 8);
	int MidRow_1 = FMath::RandRange(0, 1);
	int MidRow_2 = FMath::RandRange(3, 4);
	
	FVector2D MidPosition_1 = FVector2D(StartPos.X + GridSize.X * MidCol_1 * 2, StartPos.Y + GridSize.Y * MidRow_1 * 2);
	FVector2D MidPosition_2 = FVector2D(StartPos.X + GridSize.X * MidCol_2 * 2, StartPos.Y + GridSize.Y * MidRow_2 * 2);

	int MidLevelIndex_1 = FMath::RandRange(0, BattleLevelInfo_2.LevelMapName.Num()-1); // 中间等级的关卡
	int MidLevelIndex_2 = FMath::RandRange(0, BattleLevelInfo_2.LevelMapName.Num()-1);
	int MidLevelIndex_3 = FMath::RandRange(0, BattleLevelInfo_2.LevelMapName.Num()-1);
	FName MidLevelName_1 = BattleLevelInfo_2.LevelMapName[MidLevelIndex_1];
	FName MidLevelName_2 = BattleLevelInfo_2.LevelMapName[MidLevelIndex_2];
	FName MidLevelName_3 = BattleLevelInfo_2.LevelMapName[MidLevelIndex_3];
	
	CreateNode(NodeMap, FVector2d(1.5, 1.5), MidPosition_1, MidCol_1, MidRow_1, Battle, Lock, MidLevelName_1);
	CreateNode(NodeMap, FVector2d(1.5, 1.5), MidPosition_2, MidCol_2, MidRow_2, Battle, Lock, MidLevelName_2);
	ColMap.Emplace(MidCol_1, 1);
	ColMap.Emplace(MidCol_2, 1);

	for(int X = 6; X <= 8; X++)
	{
		if(!ColMap.Contains(X))
		{
			int MidRow_3 = FMath::RandRange(0, 4);
			FVector2D MidPosition_3 = FVector2D(StartPos.X + GridSize.X * X * 2, StartPos.Y + GridSize.Y * MidRow_3 * 2);
			ColMap.Emplace(X, 1);
			CreateNode(NodeMap, FVector2d(1.5, 1.5), MidPosition_3, X, MidRow_3, Battle, Lock, MidLevelName_3);
			break;
		}
	}

	// 左右各六列
	for(int X = 0; X < 15; X++)
	{
		if(X >= 6 && X <= 8) // 避开中间
			continue;
		
		for(int Y = 0; Y < 5; Y++)
		{
			if(ColMap.Contains(X) && ColMap[X] >= 2) // 每列最多2个节点
				break;

			int StartCheckPos_X = X-1 < 0 ? X : X-1;
			int EndCheckPos_X = X+1 >= 15 ? X : X+1;
			int StartCheckPos_Y = Y-1 < 0 ? Y : Y-1;
			int EndCheckPos_Y = Y+1 >= 5 ? Y : Y+1;
			int CountNode = 0;
			bool CanCreate = true;
			for(int i = StartCheckPos_X; i <= EndCheckPos_X; i++)
			{
				for(int j = StartCheckPos_Y; j <= EndCheckPos_Y; j++)
				{
					if(NodeMap[i][j].bExist)
						CountNode++;
					if(NodeMap[i][j].AroundCnt >= 2) // 周围点个数大于2，不能生成
						CanCreate = false;
				}
			}

			if(CountNode <= 2 && CanCreate) // 邻位最多有两个节点
			{
				if(FMath::RandRange(0, 2) == 0) // 随机一下
				{
					FName LevelName = "";
					
					NodeType nodeType = FMath::RandRange(0, 9) <= 7 ? Battle : Shop;
					
					if(nodeType == Battle)
					{
						if(X <= 4) // 低难度
						{
							int LevelIndex = FMath::RandRange(0, BattleLevelInfo_1.LevelMapName.Num()-1);
							LevelName = BattleLevelInfo_1.LevelMapName[LevelIndex];
						}
						else if(X <= 9) // 中难度
						{
							int LevelIndex = FMath::RandRange(0, BattleLevelInfo_2.LevelMapName.Num()-1);
							LevelName = BattleLevelInfo_2.LevelMapName[LevelIndex];
						}
						else // 高难度
						{
							int LevelIndex = FMath::RandRange(0, BattleLevelInfo_3.LevelMapName.Num()-1);
							LevelName = BattleLevelInfo_3.LevelMapName[LevelIndex];
						}
					}
					
					else if(nodeType == Shop)
					{
						if(X <= 4) // 低难度
						{
							int LevelIndex = FMath::RandRange(0, ShopLevelInfo_1.LevelMapName.Num()-1);
							LevelName = ShopLevelInfo_1.LevelMapName[LevelIndex];
						}
						else if(X <= 9) // 中难度
						{
							int LevelIndex = FMath::RandRange(0, ShopLevelInfo_2.LevelMapName.Num()-1);
							LevelName = ShopLevelInfo_2.LevelMapName[LevelIndex];
						}
						else // 高难度
						{
							int LevelIndex = FMath::RandRange(0, ShopLevelInfo_3.LevelMapName.Num()-1);
							LevelName = ShopLevelInfo_3.LevelMapName[LevelIndex];
						}
					}
					
					FVector2D NodePosition = FVector2D(StartPos.X + GridSize.X * X * 2, StartPos.Y + GridSize.Y * Y * 2);
					CreateNode(NodeMap, FVector2d(1.5, 1.5), NodePosition, X, Y, nodeType, Lock, LevelName, CountNode);
					if(ColMap.Contains(X))
						ColMap[X]++;
					else
						ColMap.Emplace(X, 1);

					// 周围点增加一个周围点个数
					for(int i = StartCheckPos_X; i <= EndCheckPos_X; i++)
						for(int j = StartCheckPos_Y; j <= EndCheckPos_Y; j++)
							NodeMap[i][j].AroundCnt++;
				}
			}
		}
	}
}

void ULevelMapWidgetController::GenerateLevelLine()
{
	LineCnt = 0;
	
	std::vector<FLine> Lines;
	std::vector<FNode*> Nodes;

	for(auto& Node : NodeData)
	{
		Nodes.push_back(&Node.Value);
	}

	// 所有可能的线
	for(int i = 0; i < Nodes.size()-1; i++)
	{
		for(int j = i+1; j < Nodes.size(); j++)
		{
			float distance = FVector2D::Distance(Nodes[i]->point, Nodes[j]->point);
			if(distance <= LineLenThreshold)
			{
				FLine line(-1, Nodes[i], Nodes[j], distance);
				Lines.push_back(line);
			}
		}
	}
	
	// 线两两比较，重叠、交叉时剔除长的线
	std::set<int> LinesToRemove;
	for(int i = 0; i < Lines.size()-1; i++)
	{
		for(int j = i+1; j < Lines.size(); j++)
		{
			if(IsIntersecting(Lines[i].node_1->point, Lines[i].node_2->point, Lines[j].node_1->point, Lines[j].node_2->point)) // 相交 // 注意这里使用索引而不是实际位置去求相交，因为实际位置会有精度问题，表现不是太好。
			{
				if(Lines[i].lineLenth < Lines[j].lineLenth)
					LinesToRemove.emplace(j);
				else
					LinesToRemove.emplace(i);
			}
		}
	}
	
	for(int i = 0; i < Lines.size(); i++)
	{
		if(LinesToRemove.contains(i))
			continue;

		Lines[i].LineIndex = LineCnt;
		LineData.Emplace(LineCnt, Lines[i]);
		LineCnt++;

		// 线的两端都拥有这线
		NodeData[Lines[i].node_1->NodeIndex].NeighborLines.Add(Lines[i].LineIndex);
		NodeData[Lines[i].node_2->NodeIndex].NeighborLines.Add(Lines[i].LineIndex);

		// 线的两端互相拥有
		NodeData[Lines[i].node_1->NodeIndex].NeighborNodes.Add(Lines[i].node_2->NodeIndex);
		NodeData[Lines[i].node_2->NodeIndex].NeighborNodes.Add(Lines[i].node_1->NodeIndex);
		
		OnAddLines.Broadcast(Lines[i]);
	}
}

void ULevelMapWidgetController::UpdateLevel(int CurNodeIndex, int LastNodeIndex)
{
	// 上一轮可行的点，本轮不可行了
	if(LastNodeIndex != -1)
	{
		FNode& LastNode = NodeData[LastNodeIndex];
		LastNode.state = NodeState::CompleteAndForbid;
		OnUpdateNode.Broadcast(LastNode);

		for(int i = 0; i < LastNode.NeighborNodes.Num(); i++)
		{
			int nodeId = LastNode.NeighborNodes[i];
			if(NodeData[nodeId].state == NodeState::Complete)
				NodeData[nodeId].state = NodeState::CompleteAndForbid;
			else if(NodeData[nodeId].state == NodeState::Incomplete)
				NodeData[nodeId].state = NodeState::Lock;

			OnUpdateNode.Broadcast(NodeData[nodeId]);
		}
		for(int i = 0; i < LastNode.NeighborLines.Num(); i++)
		{
			int lineId = LastNode.NeighborLines[i];
			LineData[lineId].state = Forbid;

			OnUpdateLines.Broadcast(LineData[lineId]);
		}
	}

	// 本轮可行的点（一局后刚过的那一关）
	FNode& CurNode = NodeData[CurNodeIndex];
	CurNode.state = NodeState::Complete;
	OnUpdateNode.Broadcast(CurNode);
	for(int i = 0; i < CurNode.NeighborNodes.Num(); i++)
	{
		int nodeId = CurNode.NeighborNodes[i];
		if(NodeData[nodeId].state == NodeState::CompleteAndForbid)
			NodeData[nodeId].state = NodeState::Complete;
		else if(NodeData[nodeId].state == NodeState::Lock)
			NodeData[nodeId].state = NodeState::Incomplete;

		OnUpdateNode.Broadcast(NodeData[nodeId]);
	}
	for(int i = 0; i < CurNode.NeighborLines.Num(); i++)
	{
		int lineId = CurNode.NeighborLines[i];
		LineData[lineId].state = CanMove;

		OnUpdateLines.Broadcast(LineData[lineId]);
	}
}

void ULevelMapWidgetController::CreateNode(TArray<TArray<FNode>>& NodeMap, FVector2D Size, FVector2D Position, int X, int Y, NodeType type, NodeState state, FName LevelName, int ArountCnt)
{
	if(NodeMap[X][Y].NodeIndex >= 0) // 不能重复创建
		return;
	
	const FNode node(NodeCnt, X, Y, Position, Size, type, state, LevelName, ArountCnt);
	NodeMap[X][Y] = node;
	NodeData.Emplace(NodeCnt, node);
	NodeCnt++;
	
	OnUpdateNode.Broadcast(node);
}

float ULevelMapWidgetController::CrossProduct(FVector2D p1, FVector2D p2, FVector2D p3)
{
	return (p2.X - p1.X) * (p3.Y - p1.Y) - (p2.Y - p1.Y) * (p3.X - p1.X);
}

bool ULevelMapWidgetController::IsOnSegment(FVector2D a, FVector2D b, FVector2D p)
{
	if (CrossProduct(a, b, p) != 0)
		return false;
	
	return std::min(a.X, b.X) <= p.X-0.1 && p.X+0.1 <= std::max(a.X, b.X) &&
		   std::min(a.Y, b.Y) <= p.Y && p.Y <= std::max(a.Y, b.Y);
}

bool ULevelMapWidgetController::IsIntersecting(FVector2D p1, FVector2D p2, FVector2D p3, FVector2D p4)
{
	// 计算叉积
	int d1 = CrossProduct(p3, p4, p1);
	int d2 = CrossProduct(p3, p4, p2);
	int d3 = CrossProduct(p1, p2, p3);
	int d4 = CrossProduct(p1, p2, p4);

	// 如果叉积符号不同，则线段相交
	if (d1 * d2 < 0 && d3 * d4 < 0) {
		return true;
	}

	// 如果叉积为0，检查是否在对方线段上
	if (d1 == 0 && IsOnSegment(p3, p4, p1)) return true;
	if (d2 == 0 && IsOnSegment(p3, p4, p2)) return true;
	if (d3 == 0 && IsOnSegment(p1, p2, p3)) return true;
	if (d4 == 0 && IsOnSegment(p1, p2, p4)) return true;

	return false;
}
