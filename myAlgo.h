/* Copyright (C) 2019 Joshua J. Daymude, Robert Gmyr, and Kristian Hinnenthal.
 * The full GNU GPLv3 can be found in the LICENSE file, and the full copyright
 * notice can be found at the top of main/main.cpp. */

#ifndef AMOEBOTSIM_ALG_MYALGO_H
#define AMOEBOTSIM_ALG_MYALGO_H

#include "core/amoebotparticle.h"
#include "core/amoebotsystem.h"

#include "core/amoebotparticle.h"
#include "core/amoebotsystem.h"

class MyAlgoParticle : public AmoebotParticle {
  public:
    enum class State {
        Seed,
        Active,
        Inactive,
        Root,
        Idle
    };

    enum class ColorState {
      Red,
      Orange,
      Yellow,
      Green,
      Blue,
      Indigo,
      Violet
    };

    MyAlgoParticle(const Node head, const int globalTailDir, const int orientation, AmoebotSystem& system, State s);
    void activate() override;
    QString inspectionText() const override;

    void seed();
    /* functions for seed, sends tokens that are first in clockwise direction
     * from the line direction.
     */
    void sendLineDirectionToken();

    void active();
    void inactive();
    void root();
    void idle();

    int labelOfNodeWithState(State);

    int headMarkColor() const override;

    template<class TokenType>
    void sendToken(TokenType,int);

    void randColor();

    void broadcastCoordinateToken();

protected:
  struct coordinateToken : public Token {
    coordinateToken(int newOrien = -1) { newOrientation = newOrien; }
    int newOrientation;
  };

  struct lineDirectionToken : public Token {
    lineDirectionToken(int lineDirec = 0) { lineDirection = lineDirec; }
    int lineDirection;
  };

  struct followDirectionToken : public Token {
    followDirectionToken(int fD = 0) { followDirection = fD; }
    int followDirection;
  };

  State state;
  ColorState color;

  unsigned int lineDirection;
  unsigned int followDirection;

  private:
    friend class MyAlgoSystem;
};

class MyAlgoSystem : public AmoebotSystem {
  public:
    MyAlgoSystem(unsigned int numParticles = 30);
  private:
    void placeParticles(unsigned int);
};

#endif // MYALGO_H
