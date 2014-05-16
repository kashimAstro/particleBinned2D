#include "testApp.h"

void testApp::setup(){
	ofSetVerticalSync(true);
	int binPower = 5;

	padding = 128;
	particleSystem.setup(ofGetScreenWidth() + padding * 2, ofGetScreenHeight() + padding * 2, binPower);

	kBinnedParticles = 1800;
	for(int i = 0; i < kBinnedParticles; i++) {
		float x = ofRandom(0, ofGetScreenWidth()) + padding;
		float y = ofRandom(0, ofGetScreenHeight()) + padding;
		BinnedParticle particle(x, y, 0, 0);
		particleSystem.add(particle);
	}

	timeStep = 100;
	isMousePressed = false;
	slowMotion = true;
	particleNeighborhood = 64;
	particleRepulsion = .5;
	centerAttraction = .01;
	drawBalls = true;
}

void testApp::update(){
}

void testApp::draw(){
	ofBackground(0);
	
	particleSystem.setTimeStep(timeStep);

	ofSetColor(255);
	particleSystem.setupForces();
	
	ofPushMatrix();
	ofTranslate(-padding, -padding);
	
	if(!drawBalls) {
		ofSetLineWidth(1);
		glBegin(GL_LINES);
	}
	for(int i = 0; i < particleSystem.size(); i++) {
		BinnedParticle& cur = particleSystem[i];
		particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
		cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
		cur.addDampingForce();
	}
	if(!drawBalls) {
		glEnd();
	}
	
	particleSystem.addRepulsionForce(mouseX + padding, mouseY + padding, 200, 1);
	if(isMousePressed) {
		particleSystem.addAttractionForce(particleSystem.getWidth() / 2, particleSystem.getHeight() / 2, particleSystem.getWidth() * 100, centerAttraction);
	}
	particleSystem.update(ofGetLastFrameTime());
	if(drawBalls) {
		for(int i = 0; i < particleSystem.size(); i++) {
			ofCircle(particleSystem[i].x, particleSystem[i].y, 4);
		}
	}
	
	ofPopMatrix();

	ofSetColor(255);
	ofDrawBitmapString(ofToString(kBinnedParticles) + " particles", 32, 32);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
}

void testApp::keyPressed(int key){
	if(key == 'p') {
		ofSaveScreen(ofToString(ofGetMinutes()) + "_" + ofToString(ofGetFrameNum()) + ".png");
	}
	if(key == 's') {
		slowMotion = !slowMotion;
		if(slowMotion)
			timeStep = 10;
		else
			timeStep = 100;
	}
	if(key == 'b') {
		drawBalls = !drawBalls;
	}
}

void testApp::mousePressed(int x, int y, int button){
	isMousePressed = true;
}

void testApp::mouseReleased(int x, int y, int button){
	isMousePressed = false;
}
