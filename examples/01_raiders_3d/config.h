// *************************************************************
// File:    config.h
// Descr:   enumerators
// Author:  Novoselov Anton @ 2017
// URL:     https://github.com/ans-hub/game_console
// *************************************************************

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

namespace anshub {

namespace cfg {

  constexpr const char* kBackgroundMusic = "resources/sounds/background.mp3";

  constexpr const char* kShotSnd = "resources/sounds/shot.mp3";
  constexpr const char* kExplodeSnd = "resources/sounds/explode.mp3";
  constexpr const char* kWingSnd = "resources/sounds/wing.mp3";
  constexpr const char* kScratchSnd = "resources/sounds/scratch.mp3";
  constexpr const char* kEnemySnd = "resources/sounds/enemy.mp3";
  constexpr const char* kAlarmSnd = "resources/sounds/alarm.mp3";

namespace color {

  constexpr int kBlack = 0;
  constexpr int kLightBlue = (54<<24)|(47<<16)|(37<<8)|255;
  constexpr int kDeepRed = (24<<24)|(122<<16)|(121<<8)|255;
  constexpr int kDeepGray = (50<<24)|(50<<16)|(50<<8)|50;
  constexpr int kDeepYellow = (0<<24)|(70<<16)|(89<<8)|81;
  constexpr int kBrightYellow = (0<<24)|(230<<16)|(230<<8)|230;
  constexpr int kMidOrange = (0<<24)|(88<<16)|(196<<8)|67;

}

  // Color settings

  constexpr float kMinBrightness = 0.7;
  constexpr float kMaxBrightness = 2.5;
  constexpr int kSpaceColor = color::kBlack;
  constexpr int kStarColor = color::kDeepGray;
  constexpr int kShipColor = color::kDeepYellow;
  constexpr int kExplColor = color::kBrightYellow;
  constexpr int kCannonColor = color::kDeepRed;
  constexpr int kEnemyShotColor = color::kLightBlue;
  constexpr int kAimColor = color::kMidOrange;

  // Player (viewport) settings

  constexpr int kPlayerW = 40;
  constexpr int kPlayerH = 40;
  constexpr int kStartVelocity = 43;
  constexpr int kDeadVelocity = 5;
  constexpr int kMaxVelocity = 95;
  constexpr int kCrossLen = 35;
  constexpr int kCrossVel = 5;
  constexpr int kCannonWait = 10;
  constexpr int kPlayerLife = 100;
  constexpr int kAlarmLife = 25;      // when the alarm is switch on
  
  // World settings (optimized for screen 800x600)

  constexpr int kFillColor = color::kBlack;
  constexpr int kStarsCnt = 500;
  constexpr int kStarTrack = 20;      // coeff. to eval star tracks
  constexpr int kNearZ = 10;
  constexpr int kStarFarZ = 2000;  
  constexpr int kShipFarZ = 20000;

  // Enemy settings

  constexpr int kAudibleShip = 1600;
  constexpr int kEnemyVelocity = 100;
  constexpr int kDebrisCnt = 4;       // coeff. to multi destroyed ship edges
  constexpr int kMinShotDist = 10000;
  constexpr int kEnemyAttackZ = 50;   // enemy attack aim in z coordinate
  constexpr int kEnemyStrenght = 1;
  constexpr int kEnemyShotVel = -800;

} // namespace cfg

} // namespace anshub

#endif  // GAME_CONFIG_H