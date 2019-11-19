#pragma once

#include <chrono>

#include "TouchManager.h"
#include "ofMain.h"

class PortConfig
{

public:
	PortConfig();
	~PortConfig();

	void update( double dt );
	void draw();

	inline void setup( bool configOnStart )
	{
		if ( configOnStart ) {
			startPortConfig();
		}
	}

	inline void startPortConfig()
	{
		m_isConfigActive = true;
		m_isDrawActive   = true;
		m_activeNum      = 0;

		m_configuredPorts.clear();
	}

	inline void toggleDisplayPorts()
	{
		m_isDrawActive = !m_isDrawActive;
	}

	ofEvent<bool> onPortConfigStart;
	ofEvent<bool> onPortConfigComplete;

private:
	void configureForPort( int x, int intendedPort, int acutalPort );

	void touchAdded( TouchAtPort& touch );
	void touchUpdated( TouchAtPort& args );
	void touchRemoved( TouchAtPort& args );

	bool m_isConfigActive{false};
	bool m_isDrawActive{false};

	int m_activeNum{0};

	bool m_isTouched{false};
	float m_touchDownTime{0.0f};
	float m_touchReqTime{1.0f};

	TouchAtPort lastTouch{{0, 0}, 0};

	std::map<int, glm::vec2> m_newOffsets;
	std::vector<int> m_configuredPorts;
};
