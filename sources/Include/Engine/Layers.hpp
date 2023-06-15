#pragma once

#include <string>
#include <unordered_map>
#include <Utils/Singleton.h>

enum class Layer
{
	None	= 0,
	SelfInteraction = 1 << 0,
	Player  = 1 << 2,
	MaskOnPause = 1 << 3,
	Ammos = 1 << 4,

	Default = 1 << 1 | SelfInteraction | Player | Ammos
};


struct LayerData
{
	Layer layer = Layer::Default;
	std::string name = "Default";
};

class LayerDatas : public Singleton<LayerDatas>
{
private:

	//	Private Functions
	//	-----------------

	void setLayer(std::string name, Layer id);

public:

	//	Public Variables
	//	----------------

	std::unordered_map<std::string, LayerData> m_layerList;

	//	Constructor
	//	-----------

	LayerDatas();

};


bool layersCanInteract(Layer a, Layer b);