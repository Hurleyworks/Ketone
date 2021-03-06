
// This source file was auto-generated by ClassMate++
// Created: 22 Feb 2019 8:55:26 pm
// Copyright (c) 2019, HurleyWorks

#include "berserkpch.h"
#include "RenderStack.h"

void RenderStack::pushLayer(RenderLayerRef & layer)
{
	layers.emplace(layers.begin() + layerInsertIndex, layer);
	layerInsertIndex++;
}

void RenderStack::pushOverlay(RenderLayerRef & overlay)
{
	layers.emplace_back(overlay);
}

void RenderStack::popLayer(RenderLayerRef & layer)
{
	auto it = std::find(layers.begin(), layers.end(), layer);
	if (it != layers.end())
	{
		layers.erase(it);
		layerInsertIndex--;
	}
}

void RenderStack::popOverlay(RenderLayerRef & overlay)
{
	auto it = std::find(layers.begin(), layers.end(), overlay);
	if (it != layers.end())
		layers.erase(it);
}


