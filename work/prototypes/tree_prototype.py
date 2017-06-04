from turtle import *

class Rule:
    def __init__(self, char, transform, left_context=None, right_context=None):
        self.char = char
        self.transform = transform
        self.left_context = left_context
        self.right_context = right_context
    
    def is_not_none(self, context):
        return context is not None
    
    def matches(self, char, left, right):
        char_match = (char == self.char)
        right_match = (right == self.right_context)
        left_match = (left == self.left_context)
        
        if self.is_not_none(self.left_context) and self.is_not_none(self.right_context):
            return char_match and left_match and right_match
        elif self.is_not_none(self.left_context):
            return char_match and left_match
        elif self.is_not_none(self.right_context):
            return char_match and right_match
        else:
            return char_match

class LSystem:
    def __init__(self, axiom, rules):
        self.strings = [axiom]
        self.rules = rules
        self.current_string = axiom
        self.next_string = ""

    def generate(self):
        count = 0
        for char in self.current_string:
            left = self.current_string[count-1] if count-1 > -1 else None
            right = self.current_string[count+1] if count+1 < len(self.current_string) else None
            
            rule_match = False
            for r in self.rules:
                if r.matches(char, left, right):
                    rule_match = True
                    self.next_string += r.transform
                    break
            
            if not rule_match:
                self.next_string += char
                
            count += 1

        self.current_string = self.next_string
        self.next_string = ""
        
        self.strings.append(self.current_string)

        return self.strings
    
class Tree:
    def __init__(self, tree, angle, length):
        self.tree = tree
        self.angle = angle
        self.length = length
    
    def draw(self):
        positions = []
        headings = []
        
        for sentence in self.tree:
            for c in sentence:
                if c == "F":
                    pendown()
                    forward(self.length)
                    penup()
                elif c == "G":
                    pendown()
                    forward(self.length)
                    penup()
                elif c == "+":
                    left(self.angle)
                elif c == "-":
                    right(self.angle)
                elif c == "[":
                    positions.append(position())
                    headings.append(heading())
                elif c == "]":
                    setpos(positions.pop())
                    setheading(headings.pop())

rules = [
    Rule("X", "F[-X][X]F[-X]+FX"),
    Rule("F", "FF"),
]

lsys = LSystem("X", rules)
for i in range(0, 4):
    lsys.generate()

pen({
    'speed': 10
})

print(lsys.strings[-1])

t = Tree([lsys.strings[-1]], 25, 4)
t.draw()