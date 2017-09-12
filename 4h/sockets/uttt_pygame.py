from uttt_data import *
from pygame_game import PygameGame
import pygame, pygame.locals
import uttt_data

class UTTTGame(PygameGame):

    def __init__(self, width_px, height_px, frames_per_second, data, send_queue):
        # PygameGame sets self.width and self.height        
        PygameGame.__init__(self, "Ultimate Tic Tac Toe", width_px, height_px, frames_per_second)
        self.data = data
        self.send_queue = send_queue
        self.game_over = False
        return

    def handle_state(self):
        if self.data:
            state = self.data.GetState()
            if state in [ uttt_data.STATE_SHOW_SIGNUP, uttt_data.STATE_WAIT_SIGNUP, 
                          uttt_data.STATE_SIGNUP_FAIL_USERNAME,
                          uttt_data.STATE_SHOW_LOGIN, uttt_data.STATE_WAIT_LOGIN, uttt_data.STATE_LOGIN_FAIL,
                          uttt_data.STATE_SIGNUP_FAIL_EMAIL, uttt_data.STATE_SIGNUP_FAIL_PASSWORD,
                          uttt_data.STATE_SIGNUP_FAIL_PASSWORD_UNMATCHED, uttt_data.STATE_SIGNUP_OK ]:
                # minimize window
                #pygame.display.iconify()
                if self.screen.get_size() != ( 1, 1 ):
                    print "shrink"
                    self.screen = pygame.display.set_mode(
                        # set the size
                        (1, 1),
                        # use double-buffering for smooth animation
                        pygame.DOUBLEBUF |
                        # apply alpha blending
                        pygame.SRCALPHA |
                        # allow resizing
                        pygame.RESIZABLE)
                
            elif state in [ uttt_data.STATE_WAIT_GAME, uttt_data.STATE_SHOW_GAME,
                            uttt_data.STATE_TURN_FAILED, uttt_data.STATE_WAIT_TURN ]:
                # unminimize window
                if self.screen.get_size() != ( self.width, self.height ):
                    print "WHAT?  pygame doesn't support unminimize?"
                    self.screen = pygame.display.set_mode(
                        # set the size
                        (self.width, self.height),
                        # use double-buffering for smooth animation
                        pygame.DOUBLEBUF |
                        # apply alpha blending
                        pygame.SRCALPHA |
                        # allow resizing
                        pygame.RESIZABLE)
            elif state in [ uttt_data.STATE_SOCKET_CLOSED, uttt_data.STATE_SOCKET_ERROR,
                            uttt_data.STATE_ERROR, uttt_data.STATE_GAME_OVER ]:
                # close
                self.game_over = True
                print "Socket closed, or other error, pygame will quit."
                pygame.quit()
            elif state in [ uttt_data.STATE_SOCKET_OPEN ]:
                # what should I do?
                pass
            else:
                print "Unknown state in pygame: ", state

        return

    def is_my_turn(self):
        next_player = self.data.GetNextPlayer()
        player = self.data.GetPlayer()
        return next_player == player and next_player != uttt_data.PLAYER_N
        
    def is_your_turn(self):
        next_player = self.data.GetNextPlayer()
        player = self.data.GetPlayer()
        return next_player != player and next_player != uttt_data.PLAYER_N

    def is_board_next(self, board):
        next_board = self.data.GetNextBoard()
        return next_board == board or next_board == uttt_data.BOARD_ANY
        
    def send_turn_if_legal(self, board, position):
        # Legal values for board, position?
        if board < 0 or board > 8 or position < 0 or position > 8:
            print "Bad board=%d or position=%d" % (int(board), int(position))
            return False
        # Game on, and not waiting for an update?
        if self.data.GetState() != uttt_data.STATE_SHOW_GAME:
            print "Bad state=%d" % (int(self.data.GetState()), )
            return False
        # Our turn?
        if not self.is_my_turn():
            print "Bad turn next_player=%s != player=%s" % (str(self.data.GetNextPlayer()),
                                                            str(self.data.GetPlayer()))
            return False
        # Legal board?
        if not self.is_board_next(board):
            print "Bad next_board next_board=%d != board=%d" % (int(self.data.GetNextBoard()), int(board))
            return False
        # Board not won?
        if self.data.GetBoardOwner(board) != uttt_data.PLAYER_N:
            print "Bad board_owner(board=%d)=%s" % (int(board), str(self.data.GetBoardOwner(board)))
            return False
        # Position empty?
        if self.data.GetMarker(board, position) != uttt_data.PLAYER_N:
            print "Bad marker(board=%d, position=%d)=%s" % (int(board), int(position),
                                                            str(self.data.GetMarker(board, position)))
            return False
        # Data connection and queue open?
        if (not self.data) or (not self.send_queue):
            print "Bad data=%s or send_queue=%s" % (str(self.data), str(self.send_queue))
            return False
        # OK, send it
        text = self.data.SendTurn(board, position)
        print "pygame: queuing: %s" % (text, )
        self.send_queue.put(text)
        return True

    def screen_position_to_board_and_position(self, mouse_position):
        board, position = -1, -1
        mX,mY = mouse_position[0], mouse_position[1]
        if mY < self.board_top:
            return (board, position)
        col = mX / (self.width/9)
        row = (mY - self.board_top) / ((self.height - self.board_top) / 9)
        board = 3 * (row / 3) + (col / 3)
        position = 3 * (row % 3) + (col % 3)
        
        return (board, position)

    def game_logic(self, keys, newkeys, buttons, newbuttons, mouse_position):
        self.handle_state()
        
        if 1 in newbuttons:
            if self.is_my_turn():
                (board, position) = self.screen_position_to_board_and_position(mouse_position)
                if self.send_turn_if_legal(board, position):
                    return
                    
        return

    def paint(self, surface):
        # Background
        rect = pygame.Rect(0,0,self.width,self.height)
        surface.fill((0,0,0),rect )
        
        # Regular Lines
        for i in range(1,9):
            pygame.draw.line(surface, (255,255,255), (0, i*self.height/9), (self.width, i*self.height/9))
        for j in range(1,9):
            pygame.draw.line(surface, (255,255,255), (j*self.width/9, 0), (j*self.height/9, self.height))

        # Board Lines
        for k in range(1,3):
            pygame.draw.line(surface, (255,255,0), (0, k*self.height/3), (self.width, k*self.height/3), 3)
        for l in range(1,3):
            pygame.draw.line(surface, (255,255,0), (l*self.width/3, 0), (l*self.height/3, self.height), 3)

        # Markers
        for board in range(9):
            for position in range(9):
                col = 3 * (board % 3) + position % 3
                row = 3 * (board / 3) + position / 3
                x = int((col + .5) * self.width / 9)
                y = int((row + .5) * self.height / 9)
                marker = self.data.GetMarker(board, position)
                if marker == uttt_data.PLAYER_X:
                    pygame.draw.circle(surface, (0,0,255), (x, y), 5)
                elif marker == uttt_data.PLAYER_O:
                    pygame.draw.circle(surface, (255,0,0), (x, y), 5)
        return

def uttt_pygame_main(data, send_queue):
    game = UTTTGame(600, 600, 30, data, send_queue)
    game.main_loop()
    return

if __name__ == "__main__":
    data = UTTTData()
    data.SetState(uttt_data.STATE_SHOW_GAME)
    data.SetBoardMarker(6, 8, uttt_data.PLAYER_X)
    data.SetBoardMarker(8, 2, uttt_data.PLAYER_O)
    data.SetNextTurn(4, uttt_data.PLAYER_X)
    data.SetThisPlayer(uttt_data.PLAYER_X, "ME")
    data.SetOtherPlayer("YOU")
    uttt_pygame_main(data, None)
