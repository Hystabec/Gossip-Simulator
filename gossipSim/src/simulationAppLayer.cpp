#include "simulationAppLayer.h"

#include "simulationCore.h"

//GS::simCore gossipCore;

	////Example sudo API
	////gossipCore.getGossipManager().startGossip(/*GossipData*/);

	//bool loop = true;
	//while (loop)
	//{
	//	loop = gossipCore.update();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}

SimLayer::SimLayer()
	: m_camController(1280.0f / 720.0f)
{
}

void SimLayer::attach()
{
	m_circleTexture = daedalusCore::graphics::Texture2D::create("resources/circle.png");
}

void SimLayer::detach()
{
}

void SimLayer::update(const daedalusCore::application::DeltaTime& dt)
{
	m_camController.update(dt);

	daedalusCore::graphics::RenderCommands::setClearColour(daedalusCore::utils::colour_vec4_to_normalized_vec4({ 36,36,36,255 }));
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());
	daedalusCore::graphics::Renderer2D::drawQuad({ { -0.5f, 0 }, { 0.5f, 0.5f }, m_circleTexture, { 0.3f, 0.8f, 0.2f, 1.0f } });
	daedalusCore::graphics::Renderer2D::drawQuad({ { 0.5f, 0 }, { 0.5f, 0.5f }, m_circleTexture, { 0.8f, 0.3f, 0.2f, 1.0f } });
	daedalusCore::graphics::Renderer2D::end();
}

void SimLayer::imGuiRender()
{
}

void SimLayer::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}
