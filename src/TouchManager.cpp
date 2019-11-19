#include "TouchManager.h"

std::vector<int> TouchManager::PORTS = {
    3333,
    3334,
    3335,
    3336,
	3337,
	3338};

std::map<int, glm::vec2> TouchManager::OFFSETS = {
    {3333, glm::vec2{0, 0}},
    {3334, glm::vec2{TouchManager::WIDTH, 0}},
    {3335, glm::vec2{TouchManager::WIDTH * 2, 0}},
    {3336, glm::vec2{0, TouchManager::HEIGHT}},
    {3337, glm::vec2{TouchManager::WIDTH, TouchManager::HEIGHT}},
    {3338, glm::vec2{TouchManager::WIDTH * 2, TouchManager::HEIGHT}}};

int TouchManager::WIDTH      = 500;
int TouchManager::HEIGHT     = 500;

bool TouchManager::SWAP_DIMS = false;
bool TouchManager::FLIP_X    = false;
bool TouchManager::FLIP_Y    = false;

void TouchManager::setup()
{
	setup( TouchManager::PORTS );
}

void TouchManager::setup( int port )
{
	setup( std::vector<int>( {port} ) );
}

void TouchManager::setup( std::vector<int> ports )
{
	for ( auto& port : ports ) {
		PortTouchListener listener;
		m_listeners[port].setup( this, port );
	}
}

glm::vec2 TouchManager::getConvertedCoord( const CursorAtPort& e )
{
	float xFinal = SWAP_DIMS ? e.cursor.getPosition().getY() : e.cursor.getPosition().getX();
	float yFinal = SWAP_DIMS ? e.cursor.getPosition().getX() : e.cursor.getPosition().getY();

	xFinal = xFinal * WIDTH + OFFSETS[e.port].x;
	yFinal = yFinal * HEIGHT + OFFSETS[e.port].y;

	xFinal = FLIP_X ? WIDTH - xFinal : xFinal;
	yFinal = FLIP_Y ? HEIGHT - yFinal : yFinal;

	//std::cout << "initial vals:" << e.cursor.getPosition().getX() << ", " << e.cursor.getPosition().getY() << '\n';
	//std::cout << "converted to:" << xFinal << ", " << yFinal << '\n';

	return glm::vec2( xFinal, yFinal );
}
