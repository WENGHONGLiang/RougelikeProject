// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "RougelikeProject/Data/RLInputConfig.h"
#include "RLEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELIKEPROJECT_API URLEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	// UserClass : WorldContext，后面是几个回调函数类型 // 在 PlayerController 里传入 InputConfig(DA) 文件，并指定对应回调
	template<class UserClass, typename PressedFuncTyp, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const URLInputConfig* InputConfig, UserClass* Object, PressedFuncTyp PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};


template <class UserClass, typename PressedFuncTyp, typename ReleasedFuncType, typename HeldFuncType>
void URLEnhancedInputComponent::BindAbilityActions(const URLInputConfig* InputConfig, UserClass* Object,
	PressedFuncTyp PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for(const FRLInputAction& Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				// 前面正常填完，后面可以跟 n 个参数，作为回调函数的参数
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if(ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if(HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}