// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelMapWidget.h"

#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"

ULevelMapWidget::ULevelMapWidget()
{

}

void ULevelMapWidget::UpdateNode(FNode node)
{
	// 创建 Node
	if(!NodeWidgetMap.Contains(node.NodeIndex))
	{
		ULevelNodeWidget* LevelNode = CreateWidget<ULevelNodeWidget>(this, LevelNodeWidgetClass);
		LevelNode->SetRenderTranslation(node.position);
		
		LevelNode->SetNode(node);
		LevelNode->AddToViewport(1);
		
		NodeWidgetMap.Emplace(node.NodeIndex, LevelNode);
	}
	// 更新 node
	else
	{
		ULevelNodeWidget* LevelNode = NodeWidgetMap[node.NodeIndex];
		LevelNode->SetNode(node);
	}
}

void ULevelMapWidget::AddLine(FLine line)
{
	UWidget* Widget = Lines->GetChildAt(line.LineIndex);
	USplineWidget* SplineWidget = Cast<USplineWidget>(Widget);
	SplineWidget->SplineInfo.TintColor = FLinearColor::Gray; // 禁止通行

	SplineWidget->SplineInfo.Points[0].Location = line.node_1->position;
	SplineWidget->SplineInfo.Points[1].Location = line.node_2->position;
	SplineWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void ULevelMapWidget::UpdateLine(FLine line)
{
	UWidget* Widget = Lines->GetChildAt(line.LineIndex);
	USplineWidget* SplineWidget = Cast<USplineWidget>(Widget);

	// TODO：可通行和禁止的美术效果
	if(line.state == LineState::CanMove)
		SplineWidget->SplineInfo.TintColor = FLinearColor::Yellow;
	else if(line.state == LineState::Forbid)
		SplineWidget->SplineInfo.TintColor = FLinearColor::Gray;
}

void ULevelMapWidget::GenerateBackground_Test()
{
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
	ULevelMapWidgetController* LevelMapWidgetController = Cast<ULevelMapWidgetController>(WidgetController);
	FVector2D StartPoint = ViewportCenter - FVector2D(15, 5) * LevelMapWidgetController->GridSize;

	for(int X = 0; X < 15; X++)
	{
		for(int Y = 0; Y < 5; Y++)
		{
			FVector2D NodePosition = FVector2D(StartPoint.X + LevelMapWidgetController->GridSize.X * X * 2, StartPoint.Y + LevelMapWidgetController->GridSize.Y * Y * 2);
			ULevelNodeWidget* LevelNode = CreateWidget<ULevelNodeWidget>(this, LevelNodeWidgetClass);
			LevelNode->SetRenderTransformPivot(FVector2D(0,0));
			LevelNode->SetRenderScale(FVector2D(0.5, 0.5));
			LevelNode->SetPositionInViewport(NodePosition);
			LevelNode->AddToViewport(1);
		}
	}
}

void ULevelMapWidget::ShowLevelMap(bool bShow)
{
	for(const auto& NodeWidget : NodeWidgetMap)
	{
		NodeWidget.Value->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

