#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	portConfig.setup( true );
	TouchManager::get().setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
	portConfig.update( 1.0 / 60.0 );
}

//--------------------------------------------------------------
void ofApp::draw()
{
	portConfig.draw();

	for (auto& kv : TouchManager::get().getTouches()) {
		ofFill();
		ofSetColor( 255, 0, 255 );
		ofDrawCircle( kv.second.pos.x, kv.second.pos.y, 3 );
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
	if (key == OF_KEY_F1) {
		portConfig.startPortConfig();
	}
	
	if (key == OF_KEY_F2) {
		portConfig.toggleDisplayPorts();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h )
{
}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg )
{
}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo )
{
}
