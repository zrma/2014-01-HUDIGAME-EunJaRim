#include "stdafx.h"
#include "DummyRender.h"
#include "Renderer.h"

DummyRender::DummyRender()
{
	Renderer::GetInstance()->RenderBegin();
	Renderer::GetInstance()->SetCamera();
}

DummyRender::~DummyRender()
{
	Renderer::GetInstance()->RenderEnd();
}
