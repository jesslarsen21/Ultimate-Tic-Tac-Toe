#!/usr/bin/python
# uttt.py - main driver for Universal Tic Tac Toe game
# Brian Lawson
# CS 3440 - Spring 2015
# 16 Jan 2015

__author__ = 'brian'

import pygame, sys, config
import game
from pygame.locals import *

class Uttt(game.Game):
	def __init__(self, name, screen_x, screen_y, frames_per_second):
		game.Game.__init__(self, name, screen_x, screen_y)
		pygame.font.init()

	def game_logic(self, keys, newkeys):
		pass

	def welcome_screen(self, surface):
		# TODO: Find common font to use for all OSes
		welcome_font = pygame.font.SysFont('Herculanum', 35)
		welcome_text1 = 'How about a nice game of'
		welcome_text2 = 'Ultimate Tic Tac Toe?'
		t1size = welcome_font.size(welcome_text1)
		t2size = welcome_font.size(welcome_text2)
		wt1surf = welcome_font.render(welcome_text1, True, (255,255,255))
		wt2surf = welcome_font.render(welcome_text2, True, (255,255,255))

		xpos = (config.SCREEN_X - t1size[0]) / 2
		ypos = (config.SCREEN_Y / 2) - t1size[1]
		surface.blit(wt1surf, (xpos, ypos))

		xpos = xpos = (config.SCREEN_X - t2size[0]) / 2
		ypos = (config.SCREEN_Y / 2)
		surface.blit(wt2surf, (xpos, ypos))


	def paint(self, surface):
		self.welcome_screen(surface)
		return

def main():
	try:
		game = Uttt(config.TITLE, config.SCREEN_X, config.SCREEN_Y, \
								config.FRAMES_PER_SECOND)
		game.main_loop()
	finally:
		pygame.quit()

if __name__ == '__main__':
	main()
