#include "AnimationNode.h"
#include "Utils.h"
#include <fstream>

AnimationNode::AnimationNode(const sf::Texture& texture, const std::string& filename)
: SpriteNode(texture) {
  load(filename);
}   

void AnimationNode::load(const std::string &filename) {
  std::ifstream file(filename.c_str());
  std::string line;
  std::string current;
  std::vector<AnimFrame> animation;
  while (getline(file, line)) {

    // Trim string
    const std::string delim = " \t";
    const char comment = '#';
    auto begin = line.find_first_not_of(delim);
    auto end = std::min(line.find_last_not_of(delim), line.find_first_of(comment));
    if (begin == std::string::npos || end == std::string::npos) line = "";
    else line = line.substr(begin, begin-end);

    if (line == "" or line[0] == '#') continue;
    std::stringstream stream(line);
    if (line[0] == 'A') {
      std::string token;
      stream >> token;
      assert(token == "ANIM");
      stream >> token;
      if (current != "") {
        assert(mAnimations.find(current) == mAnimations.end());
        mAnimations[current] = animation;
        animation.resize(0);
      }
      else {
        mCurrentAnim = token;
      }
      current = token;
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
  }
}

void AnimationNode::updateCurrent(sf::Time dt) {
  float deltaTime = dt.asSeconds();
  mCurrentTime += deltaTime;
  const std::vector<AnimFrame> CurrentFramess = mAnimations[mCurrentAnim];

  while (mCurrentTime > CurrentFramess[mCurrentFrame].mTime) {
    mCurrentTime -= CurrentFramess[mCurrentFrame].mTime;
    mCurrentFrame = (mCurrentFrame+1)%(CurrentFramess.size());

  }
  // Readjust texture tect
  updateTextureRect();
}

void AnimationNode::setSize(sf::Vector2u desiredSize) {
    mSize = desiredSize;
    updateTextureRect();
}

void AnimationNode::setAnimation(const std::string &animation) {
  if (animation != mCurrentAnim) {
    mCurrentAnim = animation;
    mCurrentFrame = 0;
    mCurrentTime = 0.0f;
    updateTextureRect();
  }
}

void AnimationNode::updateTextureRect() {
  AnimFrame frame = mAnimations[mCurrentAnim][mCurrentFrame];
  mSprite.setOrigin(sf::Vector2f(frame.mSize)/2.0f);
  mSprite.setTextureRect(sf::IntRect(frame.mPos, frame.mSize));

  float scaleX = mSize.x / float(frame.mSize.x);
  float scaleY = mSize.y / float(frame.mSize.y);
  mSprite.setScale(scaleX, scaleY);
}

void AnimationNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mSprite, states);
}
