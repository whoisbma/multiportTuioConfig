#pragma once

#include <functional>
#include <vector>

#include "ofxTuio.h"

struct TouchAtPort
{
	glm::vec2 pos;
	int port;
};

struct CursorAtPort
{
	ofxTuioCursor cursor;
	int port;
};

class PortTouchListener;

class TouchManager
{
public:
	static TouchManager& get()
	{
		static TouchManager touchMgr = TouchManager();
		return touchMgr;
	}
	~TouchManager() {}

	void setup();
	void setup( int port );
	void setup( std::vector<int> ports );
	glm::vec2 getConvertedCoord( const CursorAtPort& cursor );

	inline const std::map<std::string, TouchAtPort>& getTouches()
	{
		return m_touches;
	}

	inline void addCursorAtPort( CursorAtPort& e )
	{
		auto touch = TouchAtPort{getConvertedCoord( e ), e.port};
		ofNotifyEvent( onAddTouchAtPort, touch );

		m_touches.emplace( getCursorId( e.port, e.cursor.getSessionID() ), touch );
	}

	inline void updateCursorAtPort( CursorAtPort& e )
	{
		auto touch = TouchAtPort{getConvertedCoord( e ), e.port};
		ofNotifyEvent( onUpdateTouchAtPort, touch );

		m_touches[getCursorId( e.port, e.cursor.getSessionID() )] = touch;
	}

	inline void removeCursorAtPort( CursorAtPort& e )
	{
		auto touch = TouchAtPort{getConvertedCoord( e ), e.port};
		ofNotifyEvent( onRemoveTouchAtPort, touch );

		m_touches.erase( getCursorId( e.port, e.cursor.getSessionID() ) );
	}

	inline std::string getCursorId( long port, int cursorId )
	{
		return std::to_string( port ) + "-" + std::to_string( cursorId );
	}

	ofEvent<TouchAtPort> onAddTouchAtPort;
	ofEvent<TouchAtPort> onUpdateTouchAtPort;
	ofEvent<TouchAtPort> onRemoveTouchAtPort;

	static bool SWAP_DIMS;
	static bool FLIP_X;
	static bool FLIP_Y;

	static int WIDTH;
	static int HEIGHT;

	static std::vector<int> PORTS;
	static std::map<int, glm::vec2> OFFSETS;

private:
	TouchManager() {}

	std::map<int, PortTouchListener> m_listeners;
	std::map<std::string, TouchAtPort> m_touches;  // key = combined port and sessionId
};

class PortTouchListener
{
public:
	inline void setup( TouchManager* managerPtr, int port )
	{
		m_managerPtr = managerPtr;
		m_port       = port;
		m_receiver.setup( new ofxTuioUdpReceiver( port ) );

		ofAddListener( m_receiver.AddTuioCursor, this, &PortTouchListener::cursorAdded );
		ofAddListener( m_receiver.UpdateTuioCursor, this, &PortTouchListener::cursorUpdated );
		ofAddListener( m_receiver.RemoveTuioCursor, this, &PortTouchListener::cursorRemoved );

		m_receiver.connect( false );
		std::cout << "tuio port " << m_port << " touch listener connected? " << m_receiver.isConnected() << std::endl;
	}

	inline void cursorAdded( ofxTuioCursor& cursor )
	{
		CursorAtPort e = CursorAtPort{cursor, m_port};
		m_managerPtr->addCursorAtPort( e );
	}

	inline void cursorUpdated( ofxTuioCursor& cursor )
	{
		CursorAtPort e = CursorAtPort{cursor, m_port};
		m_managerPtr->updateCursorAtPort( e );
	}

	inline void cursorRemoved( ofxTuioCursor& cursor )
	{
		CursorAtPort e = CursorAtPort{cursor, m_port};
		m_managerPtr->removeCursorAtPort( e );
	}

private:
	int m_port{0};

	ofxTuioReceiver m_receiver;
	TouchManager* m_managerPtr;
};