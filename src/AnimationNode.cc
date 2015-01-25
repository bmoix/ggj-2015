#include "AnimationNode.h"
#include "Utils.h"
#include <fstream>

AnimationNode::AnimationNode(const sf::Texture& texture, const std::string& filename) :
    SpriteNode(texture),
    mCurrentFrame(0),
    mCurrentCycle(0),
    mCurrentAnim(""),
    mCurrentTime(0.0f),
    mTotalTime(0.0f)
    {
    load(filename);
}   

void AnimationNode::load(const std::string &filename) {
  std::ifstream file(filename.c_str());
  std::string line;
  std::string current;
  unsigned int cycles;
  std::vector<AnimFrame> animation;
  while (getline(file, line)) {

    // Trim string
    const std::string delim = " \t";
    const char comment = '#';
    auto begin = line.find_first_not_of(delim);
    auto end = std::min(line.find_last_not_of(delim), line.find_first_of(comment));
    if (begin == std::string::npos || end == std::string::npos) line = "";
    else line = line.substr(begin, begin-end);

    if (line == "" || line[0] == '#') continue;
    std::stringstream stream(line);
    if (line[0] == 'A') {
      std::string token;
      stream >> token;
      assert(token == "ANIM");
      stream >> token;
      if (current != "") {
        assert(mAnimations.find(current) == mAnimations.end());
        mAnimations[current] = animation;
        mAnimationCycles[current] = cycles;
        animation.resize(0);
      }
      else {
        mCurrentAnim = token;
      }
      current = token;
      stream >> cycles;
    }
    else {
      AnimFrame frame;
      stream >> frame.mPos.x 
             >> frame.mPos.y 
             >> frame.mSize.x 
             >> frame.mSize.y 
             >> frame.mTime;
      animation.push_back(frame);
    }
  }
  if (animation.size() > 0) {
   mAnimations[current] = animation;
   mAnimationCycles[current] = cycles;
  }
}

void AnimationNode::updateCurrent(sf::Time dt) {
    float deltaTime = dt.asSeconds();
    mCurrentTime += deltaTime;
    mTotalTime += deltaTime;
    const std::vector<AnimFrame> CurrentFrames = mAnimations[mCurrentAnim];

    bool needUpdate = true;
    while (mCurrentTime > CurrentFrames[mCurrentFrame].mTime) {
        mCurrentTime -= CurrentFrames[mCurrentFrame].mTime;
        ++mCurrentFrame;
        if (mCurrentFrame >= CurrentFrames.size()) {
            ++mCurrentCycle;
            unsigned int animationCycles = mAnimationCycles[mCurrentAnim];
            // If reached limit of animaton cycles, keep with last cycle
            if (animationCycles && mCurrentCycle >= animationCycles) {
                --mCurrentFrame;
                needUpdate = 0;
            }
            else {
                mTotalTime = 0;
                mCurrentFrame = 0;
            }
        }
    }
    // Readjust texture rect if frame has changed
    if (needUpdate) updateTextureRect();
}

void AnimationNode::setSize(sf::Vector2u desiredSize) {
    mSize = sf::Vector2f(desiredSize.x, desiredSize.y);
    updateTextureRect();
}

void AnimationNode::setAnimation(const std::string &animation) {
  if (animation != mCurrentAnim) {
    mCurrentAnim = animation;
    mCurrentFrame = 0;
    mCurrentTime = 0.0f;
    mTotalTime = 0.0f;
    updateTextureRect();
  }
}

void AnimationNode::resetAnimation() {
    mCurrentFrame = 0;
    mCurrentTime = 0.0f;
    mTotalTime = 0.0f;
    updateTextureRect();
}

float AnimationNode::getElapsedTime() const {
    return mTotalTime;
}

void AnimationNode::updateTextureRect() {
  AnimFrame frame = mAnimations[mCurrentAnim][mCurrentFrame];
  mSprite.setOrigin(std::abs(frame.mSize.x)/2.0f,std::abs(frame.mSize.y)/2.0f);
  mSprite.setTextureRect(sf::IntRect(frame.mPos, frame.mSize));

  float scaleX = mSize.x / float(frame.mSize.x);
  float scaleY = mSize.y / float(frame.mSize.y);
  mSprite.setScale(std::abs(scaleX), std::abs(scaleY));
}

void AnimationNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mSprite, states);
}

void AnimationNode::createBody(b2World* world, bool dynamic, float bbscalex, float bbscaley, float density, float rotation) {
    setupBody(world, dynamic, bbscalex, bbscaley, density, rotation);
    mBody->SetUserData(this);
}
