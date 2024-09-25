// Fill out your copyright notice in the Description page of Project Settings.

#include "BPFL_Math.h"

void UBPFL_Math::IntPlusEquals(int32& Int, int32 Add, int32& IntOut) {
	Int += Add;
	IntOut = Int;
}

void UBPFL_Math::IntMinusEquals(int32& Int, int32 Sub, int32& IntOut) {
	Int -= Sub;
	IntOut = Int;
}

void UBPFL_Math::IntMultiplyEquals(int32& Int, int32 Mul, int32& IntOut) {
	Int *= Mul;
	IntOut = Int;
}

void UBPFL_Math::IntDivideEquals(int32& Int, int32 Div, int32& IntOut) {
	Int /= Div;
	IntOut = Int;
}

void UBPFL_Math::FloatPlusEquals(float& Float, float Add, float& FloatOut) {
	Float += Add;
	FloatOut = Float;
}

void UBPFL_Math::FloatMinusEquals(float& Float, float Sub, float& FloatOut) {
	Float -= Sub;
	FloatOut = Float;
}

void UBPFL_Math::FloatMultiplyEquals(float& Float, float Mul, float& FloatOut) {
	Float *= Mul;
	FloatOut = Float;
}

void UBPFL_Math::FloatDivideEquals(float& Float, float Div, float& FloatOut) {
	Float /= Div;
	FloatOut = Float;
}

float UBPFL_Math::CalculateRadius(const float SemiMajorAxis, const float SemiMinorAxis, const float AngleInRadians) {
	{
		const float CosAngle = FMath::Cos(AngleInRadians); // 计算角度的余弦值
		const float SinAngle = FMath::Sin(AngleInRadians); // 计算角度的正弦值

		const float Numerator = SemiMajorAxis * SemiMinorAxis; // 计算公式中的分子
		const float Denominator = FMath::Sqrt(
			FMath::Square(SemiMinorAxis * CosAngle) + FMath::Square(SemiMajorAxis * SinAngle)); // 计算公式中的分母
		const float Radius = Numerator / Denominator; // 计算半径

		return Radius;
	}
}
