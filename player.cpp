#include "player.h"

Player::Player() {
    state = State::STOPPED;
}

void Player::play() {
    while (state == State::PLAYING) {

    }
}



Player::~Player() {

}

void Player::stop() {
    state = State::STOPPED;
}
