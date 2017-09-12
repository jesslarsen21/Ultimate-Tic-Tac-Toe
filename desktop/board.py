#! /usr/bin/python
# board.py - Board class
# Brian Lawson
# CS 3440 - Spring 2015
# 21 Jan 2015
#
import pygame

WHITE = (255, 255, 255)
GREY = (128, 128, 128)


def draw_board(surf, color):
	# first draw the large ulitimate board
	LEFTS = [100, 215, 0, 0]
	TOPS = [0, 0, 100, 215]
	WIDTHS = [15, 15, 330, 330]
	HEIGHTS = [330, 330, 15, 15]
	for i in range(4):
		r = pygame.Rect((LEFTS[i], TOPS[i]), (WIDTHS[i], HEIGHTS[i]))
		pygame.draw.rect(surf, color, r)

	# now draw the individual game sub-boards
	LEFTS		= [33, 65, 8, 8]
	TOPS		= [8, 8, 33, 62]
	WIDTHS	= [4, 4, 83, 83]
	HEIGHTS = [83, 83, 4, 4]

	for i in range(9):
		y = i / 3
		x = i % 3

		for i in range(4):
			r = pygame.Rect( (LEFTS[i], TOPS[i]),(WIDTHS[i], HEIGHTS[i]) )
			r = r.move(112 * x + x + 1, 112 * y + y + 1)
			pygame.draw.rect(surf, color, r)

	pygame.display.update()


def main():
	import sys

	pygame.init()
	DISPLAYSURF = pygame.display.set_mode((640, 480))
	DISPLAYSURF.fill(WHITE)

	draw_board(DISPLAYSURF, GREY)
	while True:
		for event in pygame.event.get():
			if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE \
					or event.type == pygame.QUIT:
				pygame.quit()
				sys.exit()


if __name__ == '__main__':
	main()
