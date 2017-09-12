import uttt_data
import Queue
import time

class Board:
		def __init__(self, data):
				self.markers = [ [ data.GetMarker(board, position) for position in range(9) ] for board in range(9) ]
				self.next_player = data.GetNextPlayer()
				self.next_board = data.GetNextBoard()
				self.winner = data.GetWinner()
				self.board_owners = [ data.GetBoardOwner(board) for board in range(9) ]
				self.this_player = data.GetPlayer()
				if self.this_player == uttt_data.PLAYER_X:
						self.other_player = uttt_data.PLAYER_O
				else:
						self.other_player = uttt_data.PLAYER_X
				return

		def CheckBoardWin(self, b):
				wins = [ [ 0, 1, 2 ],
								 [ 3, 4, 5 ],
								 [ 6, 7, 8 ],
								 [ 0, 3, 6 ],
								 [ 1, 4, 7 ],
								 [ 2, 5, 8 ],
								 [ 0, 4, 8 ],
								 [ 2, 4, 6 ],
						 ]
				for w in wins:
						if b[w[0]] == b[w[1]] and b[w[0]] == b[w[2]] and b[w[0]] != uttt_data.PLAYER_N:
								return b[w[0]]
				return uttt_data.PLAYER_N
				
		def MakeMove(self, move):
				board, position = move
				undo_info = (board, position, self.markers[board][position],
										 self.next_player, self.next_board, self.board_owners[board], self.winner)
				
				self.markers[board][position] = self.next_player
				if self.next_player == uttt_data.PLAYER_X:
						self.next_player = uttt_data.PLAYER_O
				else:
						self.next_player = uttt_data.PLAYER_X
				self.next_board = position
				self.board_owners[board] = self.CheckBoardWin(self.markers[board])
				if self.board_owners[self.next_board] != uttt_data.PLAYER_N:
						self.next_board = uttt_data.BOARD_ANY
				self.winner = self.CheckBoardWin(self.board_owners)

				return undo_info

		def UndoMove(self, undo_info):
				(b, p, m, np, nb, bo, w) = undo_info
				self.markers[b][p] = m
				self.next_player = np
				self.next_board = nb
				self.board_owners[b] = bo
				self.winner = w
				return

		def PositionScore(self, position, scores):
				if position in [ 0, 2, 6, 8 ]:
						score = scores[0]
				elif position in [ 1, 3, 5, 7 ]:
						score = scores[2]
				elif position in [ 4 ]:
						score = scores[1]
				else:
						score = 0
				return score
				
				
		def Evaluate(self):
				POSITION_EDGE_SCORE = 1
				POSITION_CORNER_SCORE = POSITION_EDGE_SCORE * 3
				POSITION_CENTER_SCORE = POSITION_CORNER_SCORE * 3
				BOARD_EDGE_SCORE = POSITION_EDGE_SCORE * 1000
				BOARD_CORNER_SCORE = POSITION_CORNER_SCORE * 1000
				BOARD_CENTER_SCORE = POSITION_CENTER_SCORE * 1000
				GAME_WIN_SCORE = BOARD_CENTER_SCORE * 1000
				BOARD_SCORES = [ BOARD_CORNER_SCORE, BOARD_CENTER_SCORE, BOARD_EDGE_SCORE ]
				POSITION_SCORES = [ POSITION_CORNER_SCORE, POSITION_CENTER_SCORE, POSITION_EDGE_SCORE ]
				score = 0
				if self.winner != uttt_data.PLAYER_N:
						if self.winner == self.this_player:
								score += GAME_WIN_SCORE
						elif self.winner == self.other_player:
								score -= GAME_WIN_SCORE
				else:
						for board in range(9):
								if self.board_owners[board] == self.this_player:
										score += self.PositionScore(board, BOARD_SCORES)
								elif self.board_owners[board] == self.other_player:
										score -= self.PositionScore(board, BOARD_SCORES)
								else:
										for position in range(9):
												if self.markers[board][position] == self.this_player:
														score += self.PositionScore(position, POSITION_SCORES)
												elif self.markers[board][position] == self.other_player:
														score -= self.PositionScore(position, POSITION_SCORES)
				return score
										
										

		def LegalMoves(self):
				moves = []
				if self.next_board == uttt_data.BOARD_ANY:
						board_list = range(9)
				else:
						board_list = [ self.next_board ]
				for board in board_list:
						if self.board_owners[board] != uttt_data.PLAYER_N:
								continue
						for position in range(9):
								if self.markers[board][position] != uttt_data.PLAYER_N:
										continue
								moves.append( (board, position) )
				return moves
								
						

class UTTTAI:

		def __init__(self, data, send_queue, ai_send_queue, ai_recv_queue, no_gui, ai_level):
				self.data = data
				self.send_queue = send_queue
				self.ai_send_queue = ai_send_queue
				self.ai_recv_queue = ai_recv_queue
				self.INFINITY = 1000000000
				self.no_gui = no_gui
				self.ai_level = ai_level
				return

		def ChooseMove(self, depth):
				if self.data.GetState() != uttt_data.STATE_SHOW_GAME:
						return None
				board = Board(self.data)
				alpha = - 2 * self.INFINITY
				beta = 2 * self.INFINITY
				self.evaluations = 0
				(move, value) = self.Max(board, depth, alpha + 0, beta + 0)
				print "Chose %s for %d in %d evaluations." % (str(move), int(value), int(self.evaluations))
				return move

		def Max(self, board, depth, alpha, beta):
				if depth <= 0 or board.winner != uttt_data.PLAYER_N:
						self.evaluations += 1
						return (None, board.Evaluate())
				max_value = - self.INFINITY
				max_move = None
				moves = board.LegalMoves()
				for m in moves:
						undo_info = board.MakeMove(m)
						(junk, value) = self.Min(board, depth-1, alpha + 0, beta + 0)
						board.UndoMove(undo_info)
						if value > max_value:
								max_value = value
								max_move = m
						if value > alpha:
								alpha = value
						if alpha >= beta:
								break
				return (max_move, max_value)

		def Min(self, board, depth, alpha, beta):
				if depth <= 0 or board.winner != uttt_data.PLAYER_N:
						self.evaluations += 1
						return (None, board.Evaluate())
				min_value = self.INFINITY
				min_move = None
				moves = board.LegalMoves()
				for m in moves:
						undo_info = board.MakeMove(m)
						(junk, value) = self.Max(board, depth-1, alpha + 0, beta + 0)
						board.UndoMove(undo_info)
						if value < min_value:
								min_value = value
								min_move = m
						if value < beta:
								beta = value
						if alpha >= beta:
								break
				return (min_move, min_value)

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
				print "ai: queuing: %s" % (text, )
				self.send_queue.put(text)
				return True


		def main_loop(self):
				while not (self.data.GetState() in [ uttt_data.STATE_SOCKET_CLOSED, uttt_data.STATE_SOCKET_ERROR,
																						 uttt_data.STATE_ERROR, uttt_data.STATE_GAME_OVER ]):
						try:
								if self.no_gui:
										if not (self.is_my_turn() and self.data.GetState() == uttt_data.STATE_SHOW_GAME):
												#print "AI: Waiting for my turn."
												time.sleep(.5)
												continue
										depth = self.ai_level
								else:
										depth = self.ai_send_queue.get(True, 1)

								print "AI: ChooseMove(%d)" % (depth, )
								move = self.ChooseMove(depth)										

								if self.no_gui:
										board, position = move
										if self.send_turn_if_legal(board, position):
												self.data.Display()
												print "AI: SendMove(%d,%d)" % (board, position)
								else:
										self.ai_recv_queue.put( move )
										print "AI: put to queue"

						except Queue.Empty as e:
								# not ready yet, fine
								pass
								
				board = Board(self.data)
				print "Final Score:", board.Evaluate()
				return
						
						

def uttt_ai_main(data, send_queue, ai_send_queue, ai_recv_queue, no_gui, ai_level):
		ai = UTTTAI(data, send_queue, ai_send_queue, ai_recv_queue, no_gui, ai_level)
		ai.main_loop()
		return
