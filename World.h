#pragma once
class World
{
public:
	World(void);
	~World(void);
	void simulate();
	void showWorldAxes();
private:
	float dt;			//time step;
	
};

