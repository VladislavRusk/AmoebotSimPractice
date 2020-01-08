#include "alg/myAlgo.h"

MyAlgoParticle::MyAlgoParticle(const Node head, const int globalTailDir,
                               const int orientation,
                               AmoebotSystem& system, State s)
    : AmoebotParticle(head, globalTailDir, orientation, system) {
  state = s;
  color = ColorState::Yellow;
}

void MyAlgoParticle::activate() {
  if (state == State::Seed)
    seed();
  else if (state == State::Idle)
    idle();
  else if (state == State::Root)
    root();
  else if (state == State::Active)
    active();
  else if (state == State::Inactive)
    inactive();
}

void MyAlgoParticle::idle() {
}

void MyAlgoParticle::active() {
}

void MyAlgoParticle::randColor() {
  int randomColor = randInt(0,6);
  switch (randomColor) {
    case 0: color = ColorState::Red;
      break;
    case 1: color = ColorState::Blue;
      break;
    case 2: color = ColorState::Green;
      break;
    case 3: color = ColorState::Indigo;
      break;
    case 4: color = ColorState::Orange;
      break;
    case 5: color = ColorState::Violet;
      break;
    default: color = ColorState::Yellow;
      break;
  }
}

int MyAlgoParticle::labelOfNodeWithState(State s) {
  auto pop = [s](const MyAlgoParticle& p) -> bool {
    if (p.state == s)
      return true;
    else
      return false;
  };
  return labelOfFirstNbrWithProperty<MyAlgoParticle>(pop,0);
}

void MyAlgoParticle::inactive() {
  color = ColorState::Blue;
  for (int label = 0; label < 6; label++)  {
    if (hasNbrAtLabel(label)) {
      MyAlgoParticle temp = nbrAtLabel<MyAlgoParticle>(label);
      if (temp.state == State::Seed) {
        state = State::Root;
        return;
      } else if (hasTailAtLabel(label)) {

      }
    }
  }
}



void MyAlgoParticle::seed() {
  lineDirection = randInt(0,6);
  broadcastCoordinateToken();
  sendLineDirectionToken();
  state = State::Idle;
  color = ColorState::Red;
}

void MyAlgoParticle::sendLineDirectionToken() {
  unsigned int oppositeDir = (lineDirection+3) % 6;
  int top = labelOfFirstObjectNbr(lineDirection);
  int bottom = labelOfFirstObjectNbr(oppositeDir);
  lineDirectionToken lDT(lineDirection);
  if ((top + 1 % 6) != (oppositeDir + 4 % 6) && top > -1) {
    sendToken<lineDirectionToken>(lDT, top);
  }
  if ((bottom + 1 % 6) != (lineDirection + 4 % 6) && bottom > -1) {
    sendToken<lineDirectionToken>(lDT, bottom);
  }
}

template<class TokenType>
void MyAlgoParticle::sendToken(TokenType token, int label) {
  if (hasNbrAtLabel(label)) {
    MyAlgoParticle temp = nbrAtLabel<MyAlgoParticle>(label);
    std::shared_ptr<TokenType> tokenPointer = std::make_shared<TokenType>(token);
    temp.putToken(tokenPointer);
  }
}

void MyAlgoParticle::broadcastCoordinateToken() {
  for (int i = 0; i < 6; i++) {
    if (hasNbrAtLabel(i)) {
      MyAlgoParticle* temp = &nbrAtLabel<MyAlgoParticle>(i);
      auto test = std::shared_ptr<coordinateToken>(new coordinateToken(orientation));
      temp->putToken(test);
    }
  }
}

void MyAlgoParticle::root() {
  if (hasToken<coordinateToken>()) {
    color = ColorState::Indigo;
    auto cToken = peekAtToken<coordinateToken>();
  }
  if (hasToken<lineDirectionToken>()) {
    auto lDToken = peekAtToken<lineDirectionToken>();
    lineDirection = lDToken->lineDirection;
    color = ColorState::Green;
  }
}

QString MyAlgoParticle::inspectionText() const {
  QString text;
  text += "Global Info:\n";
  text += "  head: (" + QString::number(head.x) + ", "
                      + QString::number(head.y) + ")\n";
  text += "  orientation: " + QString::number(orientation) + "\n";
  text += "  globalTailDir: " + QString::number(globalTailDir) + "\n\n";

  text += "Local Info:\n";
  text += "  state: ";
  text += [this](){
    switch(state) {
      case State::Idle: return "Idle\n";
      case State::Active: return "Active\n";
      case State::Inactive: return "Inactive\n";
      case State::Root: return "Root\n";
      case State::Seed: return "Seed\n";
    }
    return "no state\n";
  }();

  return text;
}


MyAlgoSystem::MyAlgoSystem(unsigned int numParticles) {
  placeParticles(numParticles);
}


int MyAlgoParticle::headMarkColor() const {
  switch(color) {
    case ColorState::Red:    return 0xff0000;
    case ColorState::Orange: return 0xff9000;
    case ColorState::Yellow: return 0xffff00;
    case ColorState::Green:  return 0x00ff00;
    case ColorState::Blue:   return 0x0000ff;
    case ColorState::Indigo: return 0x4b0082;
    case ColorState::Violet: return 0xbb00ff;
  }
  return -1;
}


void MyAlgoSystem::placeParticles(unsigned int numParticles) {
  int add[6][2] = {{-1,1},{-1,0},{0,-1},{1,-1},{1,0},{0,1}};
  std::set<Node> occupied;
  Node node(0,0);
  occupied.insert(node);
  insert(new MyAlgoParticle(node, -1, randDir(),*this, MyAlgoParticle::State::Seed));
  int row = 1;
  unsigned int n = sqrt(numParticles/6) + 2;
  unsigned int numP = (n*(n+1))*3;
  while (occupied.size() < numP) {
    int wall = 0;
    int x = row;
    int y = 0;
    while (wall < 6) {
      wall = wall % 6;
      int added = 0;
      while (added < row) {
        x += add[wall][0];
        y += add[wall][1];
        Node node(x,y);
        occupied.insert(node);
        insert(new MyAlgoParticle(node, -1, randDir(),*this, MyAlgoParticle::State::Inactive));
        added++;
      }
      wall++;
    }
    row++;
  }
}
