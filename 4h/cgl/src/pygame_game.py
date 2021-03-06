#
# Don't change this file.
#
import pygame, pygame.locals

class PygameGame:
    def __init__(self, name, width, height, frames_per_second):
        self.width = width
        self.height = height
        self.frames_per_second = frames_per_second
        self.on = True

        self.screen = pygame.display.set_mode(
                # set the size
                (width, height),
                # use double-buffering for smooth animation
                pygame.DOUBLEBUF |
                # apply alpha blending
                pygame.SRCALPHA |
                # allow resizing
                pygame.RESIZABLE)

        # set the title of the window
        pygame.display.set_caption(name)

    def game_logic(self, keys, newkeys, buttons, newbuttons, mouse_position):
        raise NotImplementedError()

    def paint(self, surface):
        raise NotImplementedError()

    def generate_external_events(self):
        return

    def main_loop(self):
        clock = pygame.time.Clock()
        keys = set()
        buttons = set()
        mouse_position = (1,1)

        while True:
            self.generate_external_events()
            clock.tick(self.frames_per_second)
            self.generate_external_events()

            newkeys = set()
            newbuttons = set()
            for e in pygame.event.get():
                # did the user try to close the window?
                if e.type == pygame.QUIT:
                    pygame.quit()
                    return

                # did the user just press the escape key?
                if e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE:
                    pygame.quit()
                    return

                # track which mouse buttons are currently pressed
                if e.type == pygame.MOUSEBUTTONDOWN:
                    buttons.add(e.button)
                    newbuttons.add(e.button)
                    mouse_position = e.pos

                if e.type == pygame.MOUSEBUTTONUP:
                    buttons.discard(e.button)
                    mouse_position = e.pos

                if e.type == pygame.MOUSEMOTION:
                    mouse_position = e.pos
                
                # track which keys are currently set
                if e.type == pygame.KEYDOWN:
                    keys.add(e.key)
                    newkeys.add(e.key)
                if e.type == pygame.KEYUP:
                    keys.discard(e.key)

            if self.on:
                self.generate_external_events()
                self.game_logic(keys, newkeys, buttons, newbuttons, mouse_position)
                self.generate_external_events()
                self.paint(self.screen)

            pygame.display.flip()

