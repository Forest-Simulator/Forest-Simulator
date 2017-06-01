import random
from turtle import *

random.seed(1)

random.randint(1, 5)

class Branch:
    def __init__(self, length, space):
        self.children = []
        self.length = length
        self.space = space
        
        new_length = self.length - 2
        new_space = self.space + 1
        rand_num_branches = random.randint(2, 5)
        
        if new_length > 0:
            for i in range(0, rand_num_branches):
                self.children.append(Branch(new_length, new_space))
            
    def draw(self, parent_pos):
        
        # Reset the position if this is not the root
        if parent_pos is not None:
            setpos(parent_pos[0], parent_pos[1])
            
        pendown()
        forward(self.length * 10)
        penup()
        pos = position()
    
        for branch in self.children:
            rand_angle = random.randint(-90, 90)
            right(rand_angle)
            self.children[0].draw(pos)
            right(-rand_angle)
        
    def __str__(self):
        string = ''
        
        for i in range(0, self.length):
            string += (' ' * self.space) + '|\n'
        
        for branch in self.children:
            string += str(branch)
        
        return string

class Tree:
    def __init__(self):
        self.initial_branch = Branch(10, 0)
        
    def draw(self):
        left(90)
        self.initial_branch.draw(None)
        
    def __str__(self):
        return str(self.initial_branch)
        
        
t = Tree()
t.draw()
done()