import random
from turtle import *

ignore = [
    '[', ']', '+', '-', '\\', '/', '&', '^'
]

class Rule:
    def __init__(self, char, transform, left_context=None, right_context=None, prob=None):
        self.char = char
        self.transform = transform
        self.left_context = left_context
        self.right_context = right_context
        self.prob = prob
    
    def is_not_none(self, context):
        return context is not None
    
    def matches(self, char, left, right):
        char_match = (char == self.char)
        right_match = False
        left_match = False
    
        if char_match:
            if self.right_context and right:
                final_string = self.get_complete_right_context(right)

                l = len(self.right_context)
                s = final_string[:l]
                right_match = (s == self.right_context)
            if self.left_context and left:
                final_string = self.get_complete_left_context(left)

                l = len(self.left_context)
                s = final_string[-l:]

                left_match = (s == self.left_context)

            if self.is_not_none(self.left_context) and self.is_not_none(self.right_context):
                return char_match and left_match and right_match
            elif self.is_not_none(self.left_context):
                return char_match and left_match
            elif self.is_not_none(self.right_context):
                return char_match and right_match
            
        return char_match
        
    def get_complete_right_context(self, right):
        final_string = ''
        for c in range(0, len(right)):
            s = right[c]
            if s not in ignore:
                final_string += s

        return final_string
        
    def get_complete_left_context(self, left):
        skip_stack = []
        skip = False
        final_string = ''

        for c in range(len(left)-1, -1, -1):
            s = left[c]
            if s == ']':
                skip = True
                skip_stack.append(skip)
            elif s == '[':
                if len(skip_stack) > 0:
                    skip_stack.pop()

                if len(skip_stack) == 0:
                    skip = False

            if not skip:
                if s not in ignore:
                    final_string += s
        
        return final_string[::-1]

    def __str__(self):
        string = "Rule({char}, {transform}".format(
            char=self.char,
            transform=self.transform
        )

        if self.prob:
            string += ", {prob}".format(prob=self.prob)

        string += ")"

        return string

class LSystem:
    def __init__(self, axiom, rules):
        self.strings = [axiom]
        self.rules = rules
        self.current_string = axiom
        self.next_string = ""

    def generate(self):
        count = 0
        size = len(self.current_string)
        for char in self.current_string:
            left = self.current_string[:count]
            right = self.current_string[count+1:size]
            rule_matches = []
            rule_match = False
            prob_sum = 0
            
            for r in self.rules:
                if r.matches(char, left, right):
                    if r.prob:
                        rule_matches.append(r)
                        prob_sum += r.prob
                    else:
                        self.next_string += r.transform
                        rule_match = True
                        break

            if len(rule_matches) == 0 and not rule_match:
                self.next_string += char
            else:
                accum = 0.0
                for match in rule_matches:
                    r = random.uniform(0.0, 1.0)
                    chance = (match.prob / prob_sum) + accum
                    
                    if r <= chance:
                        self.next_string += match.transform
                        break
                    accum += chance

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
                elif c == "f":
                    forward(self.length)
                elif c == "+" or c == "/" or c == "’":
                    left(self.angle)
                elif c == "-" or c == "\\" or c == "&":
                    right(self.angle)
                elif c == "|":
                    right(180)
                elif c == "[":
                    positions.append(position())
                    headings.append(heading())
                elif c == "]":
                    setpos(positions.pop())
                    setheading(headings.pop())
                elif c == "{":
                    begin_poly()
                    begin_fill()
                elif c == "}":
                    end_poly()
                    end_fill()

rules = [
    Rule("A", "[+A{.].C.}"),
    Rule("B", "[-B{.].C.}"),
    Rule("C", "GC")
]

lsys = LSystem("[A][B]", rules)
for i in range(0, 10):
    lsys.generate()

pen({
    'speed': 10
})

print(lsys.strings[-1])

t = Tree([lsys.strings[-1]], 20, 20)
t.draw()