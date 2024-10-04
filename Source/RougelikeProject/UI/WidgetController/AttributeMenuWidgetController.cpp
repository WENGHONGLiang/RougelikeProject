// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"

#include "RougelikeProject/ArributeBaseSet/AttributeSetBase.h"
#include "RougelikeProject/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAttributeSetBase* AS = CastChecked<UAttributeSetBase>(AttributeSet);

	for(auto& Pair : AS->TagsToAttributes)
	{
		// 对每个属性注册 ASC 事件
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				FRLAttributeInfo Info = AttributeInfo->FindAttributeInfoTag(Pair.Key);	// 通过属性 Tag 得到 DA 数据
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);					// 将实际属性值填入 DA 的 Value
				AttributeInfoDelegate.Broadcast(Info);									// 广播出去让 Widget 接到
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAttributeSetBase* AS = CastChecked<UAttributeSetBase>(AttributeSet);

	check(AttributeInfo);

	for(auto& Pair : AS->TagsToAttributes)
	{
		FRLAttributeInfo Info = AttributeInfo->FindAttributeInfoTag(Pair.Key);

		// Value是函数指针，能直接调用
		FGameplayAttribute Attr = Pair.Value();

		// FGameplayAttribute 是一个全局的属性声明，通过具体的 AttributeSet 得到具体值
		Info.AttributeValue = Attr.GetNumericValue(AS);

		AttributeInfoDelegate.Broadcast(Info);
	}
}
