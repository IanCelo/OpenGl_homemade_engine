#include <Engine/Layers.hpp>

bool layersCanInteract(Layer a, Layer b)
{
	int bitA = (int)a;
	int bitB = (int)b;

	if (a == b)
	{
		int i = bitA & (int)Layer::SelfInteraction;
		return  i == (int)Layer::SelfInteraction;
	}
	return bitA & bitB;
}



LayerDatas::LayerDatas()
{
	setLayer("None", Layer::None);
	setLayer("Default", Layer::Default);
	setLayer("Player", Layer::Player);
	setLayer("MaskOnPause", Layer::MaskOnPause);
	setLayer("Ammos", Layer::Ammos);
}

void LayerDatas::setLayer(std::string name, Layer id)
{
	m_layerList[name] = { id, name };
}