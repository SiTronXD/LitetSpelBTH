#include "Button.h"


class Cycler : public Button
{
private:
	std::vector<std::string> elements;
	std::string current;
public:
	Cycler(DirectX::SimpleMath::Vector2 p, int w, int h, DirectX::SimpleMath::Vector3 bClr,
		DirectX::SimpleMath::Vector3 hClr, bool hov, UIRenderer& r, AudioEngine& a);
	~Cycler();

	inline std::vector<std::string> getElements() const { return this->elements; }
	inline std::string getCurrent() const { return this->current; }

	void setElements(std::vector<std::string> elements);
	void setCurrent(std::string current);
};