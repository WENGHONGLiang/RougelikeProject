// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPFL_Math.generated.h"

/**
 * 数学库
 */
UCLASS(Blueprintable)
class UBPFL_Math : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/** int += */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "+=", Keywords = "increment integer"),
		Category = "BPFL|Math|Integer")
	static void IntPlusEquals(UPARAM(ref) int32& Int, int32 Add, int32& IntOut);
	/** int -= */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "-=", Keywords = "decrement integer"),
		Category = "BPFL|Math|Integer")
	static void IntMinusEquals(UPARAM(ref) int32& Int, int32 Sub, int32& IntOut);
	/** int *= */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "*=", Keywords = "decrement integer"),
		Category = "BPFL|Math|Integer")
	static void IntMultiplyEquals(UPARAM(ref) int32& Int, int32 Mul, int32& IntOut);
	/** int /= */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "/=", Keywords = "decrement integer"),
		Category = "BPFL|Math|Integer")
	static void IntDivideEquals(UPARAM(ref) int32& Int, int32 Div, int32& IntOut);
	/** float += */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "+=", Keywords = "increment float"),
		Category = "BPFL|Math|Float")
	static void FloatPlusEquals(UPARAM(ref) float& Float, float Add, float& FloatOut);
	/** float -= */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "-=", Keywords = "decrement float"),
		Category = "BPFL|Math|Float")
	static void FloatMinusEquals(UPARAM(ref) float& Float, float Sub, float& FloatOut);
	/** float *= */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "*=", Keywords = "decrement float"),
		Category = "BPFL|Math|Float")
	static void FloatMultiplyEquals(UPARAM(ref) float& Float, float Mul, float& FloatOut);
	/** float /= */
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "/=", Keywords = "decrement float"),
		Category = "BPFL|Math|Float")
	static void FloatDivideEquals(UPARAM(ref) float& Float, float Div, float& FloatOut);
	/**
	 * 计算椭圆在某个角度上的点到中心的距离（半径
	 * @param SemiMajorAxis 长轴
	 * @param SemiMinorAxis 短轴
	 * @param AngleInRadians 角度
	 * @return 距离（半径
	 */
	static float CalculateRadius(const float SemiMajorAxis, const float SemiMinorAxis, const float AngleInRadians);
};
