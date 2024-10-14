// Copyright Qibo Pang 2022. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "Layout/Clipping.h"


class UMGSPLINE_API FUMGSplineRenderBatch
{
public:
	FUMGSplineRenderBatch(TArray<FSlateVertex>* InSourceVertArray, TArray<SlateIndex>* InSourceIndexArray);

	void ReserveVertices(uint32 Num)
	{
		SourceVertices->Reserve(SourceVertices->Num() + Num);
	}

	void ReserveIndices(uint32 Num)
	{
		SourceIndices->Reserve(SourceIndices->Num() + Num);
	}

	void AddVertex(FSlateVertex&& Vertex)
	{
		//const int32 Index = SourceVertices->AddUninitialized(1);
		//new(SourceVertices->GetData() + Index) FSlateVertex(Forward<FSlateVertex>(Vertex));
		
		// I am not sure whether above code is safe, so I changed it like below:
		SourceVertices->Add(Vertex);

		++NumVertices;
	}

	void AddIndex(SlateIndex Index)
	{
		SourceIndices->Add(Index);
		++NumIndices;
	}

	void AddVertices(const TArray<FSlateVertex>& InVertices)
	{
		SourceVertices->Append(InVertices);
		NumVertices += InVertices.Num();
	}

	void AddIndices(const TArray<SlateIndex>& InIndices)
	{
		SourceIndices->Append(InIndices);
		NumIndices += InIndices.Num();
	}

	uint32 GetNumVertices() const
	{
		return NumVertices;
	}

	uint32 GetNumIndices() const
	{
		return NumIndices;
	}

	bool HasVertexData() const
	{
		return NumVertices > 0 && NumIndices > 0;
	}

	bool IsValidForRendering() const
	{
		return (NumVertices > 0 && NumIndices > 0);
	}

public:
	// Source Data
	TArray<FSlateVertex>* SourceVertices;

	TArray<SlateIndex>* SourceIndices;

	/** Number of vertices in the batch */
	int32 NumVertices;

	/** Number of indices in the batch */
	int32 NumIndices;

};
