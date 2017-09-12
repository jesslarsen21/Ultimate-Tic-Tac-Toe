#pragma once

#ifndef _HUMANPLAYER_H_
#define _HUMANPLAYER_H_

#include "Player.h"

class HumanPlayer : public Player
{
public:
  HumanPlayer();
  bool getMove(int &board_out, int &position_out);
  
};

#endif /* _HUMANPLAYER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
