#include "ofxGamepadHandler.h"

#ifdef TARGET_LINUX
#include "ofxGamepadLinux.h"
#endif

ofxGamepadHandler* ofxGamepadHandler::singleton;
bool ofxGamepadHandler::hasSingleton = false;

ofxGamepadHandler::ofxGamepadHandler()
{
	ofAddListener(ofEvents.update, this, &ofxGamepadHandler::update);
	updatePadList();
}

ofxGamepadHandler::~ofxGamepadHandler()
{
}

ofxGamepadHandler* ofxGamepadHandler::get()
{
	if(!hasSingleton) {
		singleton=new ofxGamepadHandler();
		hasSingleton=true;
	}
	return singleton;
}

void ofxGamepadHandler::updatePadList()
{
#ifdef TARGET_LINUX
	//check for joysticks
	ofFile file;
	for(int i=0; i<32; i++) {
		file.open("/dev/input/js"+ofToString(i));
		if(file.exists()) {
			try {
				gamepads.push_back(ofPtr<ofxGamepad>(new ofxGamepadLinux(file.getAbsolutePath())));
			} catch(std::exception& err) {
			}
		}
	}
#else
	ofLog(OF_LOG_ERROR, "SORRY, linux only for now...");
#endif
}

void ofxGamepadHandler::update(ofEventArgs &args){
	gamepadList::iterator it=gamepads.begin();
	while(it!=gamepads.end()){
		(*it)->update();
		++it;
	}
}