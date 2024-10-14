// Copyright Qibo Pang 2022. All Rights Reserved.

#include "UMGSplineRenderBatch.h"

FUMGSplineRenderBatch::FUMGSplineRenderBatch(
	TArray<FSlateVertex>* InSourceVertexArray,
	TArray<SlateIndex>* InSourceIndexArray)
	: SourceVertices(InSourceVertexArray)
	, SourceIndices(InSourceIndexArray)
	, NumVertices(0)
	, NumIndices(0)
{
}