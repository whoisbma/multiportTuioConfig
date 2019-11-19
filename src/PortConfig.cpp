#include "PortConfig.h"

PortConfig::PortConfig()
{
	ofAddListener( TouchManager::get().onAddTouchAtPort, this, &PortConfig::touchAdded );
	ofAddListener( TouchManager::get().onUpdateTouchAtPort, this, &PortConfig::touchUpdated );
	ofAddListener( TouchManager::get().onRemoveTouchAtPort, this, &PortConfig::touchRemoved );
}

PortConfig::~PortConfig()
{
	ofRemoveListener( TouchManager::get().onAddTouchAtPort, this, &PortConfig::touchAdded );
	ofRemoveListener( TouchManager::get().onUpdateTouchAtPort, this, &PortConfig::touchUpdated );
	ofRemoveListener( TouchManager::get().onRemoveTouchAtPort, this, &PortConfig::touchRemoved );
}

void PortConfig::update( double dt )
{
	if ( m_isConfigActive ) {
		if ( m_isTouched ) {
			//std::cout << ofGetElapsedTimef() << " > " << ( m_touchDownTime + m_touchReqTime ) << "?\n";
			if ( ofGetElapsedTimef() > m_touchDownTime + m_touchReqTime ) {
				std::cout << "OK\n";
				configureForPort( lastTouch.pos.x, TouchManager::PORTS[m_activeNum], lastTouch.port );
				m_touchDownTime = 0.0f;
				m_isTouched     = false;
			}
		}
	}
}

void PortConfig::draw()
{
	// draw state
	if ( m_isDrawActive ) {
		ofNoFill();
		ofSetLineWidth( 5 );

		if ( !m_isConfigActive ) {
			for ( auto& port : TouchManager::PORTS ) {
				ofSetColor( std::find( m_configuredPorts.begin(), m_configuredPorts.end(), port ) != m_configuredPorts.end() ? ofColor( 100, 210, 150 ) : ofColor( 0 ) );
				ofDrawRectangle( TouchManager::OFFSETS[port], TouchManager::WIDTH, TouchManager::HEIGHT );
				ofSetColor( 0 );
				ofDrawBitmapString(
					"port: " + std::to_string( port ),
					TouchManager::OFFSETS[port].x + 30,
					TouchManager::OFFSETS[port].y + 50 );
				ofDrawBitmapString(
					"offsets: " + std::to_string( TouchManager::OFFSETS[port].x ) + ", " + std::to_string( TouchManager::OFFSETS[port].y ),
					TouchManager::OFFSETS[port].x + 30,
					TouchManager::OFFSETS[port].y + 70 );

			}
			ofSetColor( 0 );
			ofDrawBitmapString( "press F1 to reset, F2 to close", 30, 30 );
		}

		ofSetLineWidth( 1 );
		ofFill();
	}

	if ( m_isConfigActive ) {
		ofSetColor( 255, 150 );
		ofDrawRectangle( TouchManager::OFFSETS[TouchManager::PORTS[m_activeNum]], TouchManager::WIDTH, TouchManager::HEIGHT );

		std::string timePct = m_isTouched ? ", " + std::to_string( static_cast<int>( 100 * ( ( ofGetElapsedTimef() - m_touchDownTime ) / m_touchReqTime )) ) + "%" : ", 0%";

		ofSetColor( 0 );
		ofDrawBitmapString(
		    "configuring device " + std::to_string( m_activeNum ) + timePct,
		    TouchManager::OFFSETS[TouchManager::PORTS[m_activeNum]].x + 30,
		    TouchManager::OFFSETS[TouchManager::PORTS[m_activeNum]].y + 90 );
	}
}

void PortConfig::configureForPort( int x, int intendedPort, int actualPort )
{
	if ( m_activeNum < TouchManager::PORTS.size() ) {
		std::cout << "got x " << x << ", intended port " << intendedPort << ", actual port " << actualPort << '\n';
		m_newOffsets[actualPort]         = TouchManager::OFFSETS[intendedPort];
		TouchManager::PORTS[m_activeNum] = actualPort;

		m_configuredPorts.emplace_back( actualPort );
		m_activeNum++;
	}

	if ( m_activeNum == static_cast<int>( TouchManager::PORTS.size() ) ) {
		m_activeNum           = 0;
		m_isConfigActive        = false;
		TouchManager::OFFSETS = m_newOffsets;
		std::cout << "new offsets:\n";
		int i = 0;
		for ( auto& kv : TouchManager::OFFSETS ) {
			std::cout << "\t" << kv.first << ": " << kv.second << '\n';
			i++;
		}
		bool success = true;
		ofNotifyEvent( onPortConfigComplete, success );
	}
}

void PortConfig::touchAdded( TouchAtPort& touch )
{
	if ( !m_isConfigActive ) { return; }

	if ( std::find( m_configuredPorts.begin(), m_configuredPorts.end(), touch.port ) == m_configuredPorts.end() ) {
		m_touchDownTime = ofGetElapsedTimef();
		m_isTouched     = true;
		lastTouch       = touch;
	} else {
		std::cout << "port " << touch.port << " already configured with offsets " << TouchManager::OFFSETS[touch.port] << '\n';
	}
}

void PortConfig::touchUpdated( TouchAtPort& touch )
{
}

void PortConfig::touchRemoved( TouchAtPort& touch )
{
	m_touchDownTime = 0.0f;
	m_isTouched     = false;

	lastTouch = TouchAtPort{{0, 0}, 0};
}
