// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelNodeWidget.h"

#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/GameMode/RLGameMode.h"


void ULevelNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 暂定未探索和以探索节点都一样
	btn_Inc_1->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
	btn_Inc_2->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
	btn_Inc_3->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
	btn_Inc_4->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
	btn_com_1->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
	btn_com_2->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
	btn_com_3->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
	btn_com_4->OnClicked.AddDynamic(this, &ULevelNodeWidget::OnButtonClick);
}

void ULevelNodeWidget::SetNode(FNode node)
{
	switch (node.type)
	{
	case Battle:
		BattleNodeSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		BattleNodeSwitcher->SetRenderTranslation(FVector2D(0,0));
		BattleNodeSwitcher->SetActiveWidgetIndex(node.state);
		break;
	case Shop:
		ShopNodeSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ShopNodeSwitcher->SetRenderTranslation(FVector2D(0,0));
		ShopNodeSwitcher->SetActiveWidgetIndex(node.state);
		break;
	case Start:
		StartNodeSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		StartNodeSwitcher->SetRenderTranslation(FVector2D(0,0));
		StartNodeSwitcher->SetActiveWidgetIndex(node.state);
		break;
	case End:
		EndNodeSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		EndNodeSwitcher->SetRenderTranslation(FVector2D(0,0));
		EndNodeSwitcher->SetActiveWidgetIndex(node.state);
		break;
	}

	NodeIndex = node.NodeIndex;
	LevelName = node.LevelName;
}

void ULevelNodeWidget::OnButtonClick()
{
	Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this))->StartLevel(NodeIndex, LevelName);
}
