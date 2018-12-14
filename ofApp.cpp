#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);

	this->box2d.init();
	this->box2d.setGravity(0, 0);
	this->box2d.createBounds();
	this->box2d.setFPS(60);

	for (int i = 0; i < 25; i++) {

		auto circle = make_shared<ofxBox2dCircle>();
		circle.get()->setPhysics(0.5, 1.0, 1.0);
		circle.get()->setup(this->box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), 8);
		this->circles.push_back(move(circle));

		ofColor c;
		c.setHsb(i * 10, 255, 230);
		this->circles_color.push_back(c);

		deque<ofPoint> log;
		this->circles_log.push_back(log);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->circles.size(); i++) {

		int span = 30;
		for (int x = ofGetWidth() * 0.2; x <= ofGetWidth() * 0.8; x += span) {

			for (int y = ofGetHeight() * 0.2; y <= ofGetHeight() * 0.8; y += span) {

				if ((this->circles[i]->getPosition().x > x - span * 0.5 && this->circles[i]->getPosition().x < x + span * 0.5) &&
					(this->circles[i]->getPosition().y > y - span * 0.5 && this->circles[i]->getPosition().y < y + span * 0.5))
				{
					float noise_deg = ofNoise(x * 0.0005, y * 0.0005, ofGetFrameNum() * 0.01) * 360;
					ofPoint power = ofPoint(cos(noise_deg * DEG_TO_RAD), sin(noise_deg * DEG_TO_RAD));
					this->circles[i]->addForce(power, 1.0);
				}
			}
		}
	}

	this->box2d.update();

	for (int i = 0; i < this->circles.size(); i++) {

		this->circles_log[i].push_front(this->circles[i]->getPosition());
		while (this->circles_log[i].size() > 10) {

			this->circles_log[i].pop_back();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofFill();
	ofSetColor(39);

	int span = 30;
	for (int x = ofGetWidth() * 0.2; x <= ofGetWidth() * 0.8; x += span) {

		for (int y = ofGetHeight() * 0.2; y <= ofGetHeight() * 0.8; y += span) {

			float noise_deg = ofNoise(x * 0.0005, y * 0.0005, ofGetFrameNum() * 0.01) * 360;

			ofBeginShape();
			ofVertex((x + span * 0.5) + span * 0.35 * cos(noise_deg * DEG_TO_RAD), (y + span * 0.5) + span * 0.35 * sin(noise_deg * DEG_TO_RAD));
			ofVertex((x + span * 0.5) + span * 0.35 * cos((noise_deg + 135) * DEG_TO_RAD), (y + span * 0.5) + span * 0.35 * sin((noise_deg + 135) * DEG_TO_RAD));
			ofVertex((x + span * 0.5) + span * 0.35 * cos((noise_deg + 225) * DEG_TO_RAD), (y + span * 0.5) + span * 0.35 * sin((noise_deg + 225) * DEG_TO_RAD));
			ofEndShape(true);
		}
	}

	for (int i = 0; i < this->circles.size(); i++) {

		ofFill();
		ofSetColor(this->circles_color[i]);
		ofDrawCircle(this->circles[i]->getPosition(), this->circles[i]->getRadius());

		ofNoFill();
		ofBeginShape();
		for (int log_index = 0; log_index < this->circles_log[i].size(); log_index++) {

			ofVertex(this->circles_log[i][log_index]);
		}
		ofEndShape(false);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}