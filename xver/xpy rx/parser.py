import sys
header = sys.argv[1]
target = sys.argv[2]

def flip_paras(content):
    new = ""
    parity = 1
    for letter in content:
        if "$" == letter:
            if parity:
                new += "{"
                parity = 0
            else:
                new += "}"
                parity = 1
        elif "{" == letter:
            new += "{left_bracket}" 
        elif "}" == letter:
            new += "{right_bracket}" 
        elif "\"" == letter:
            new += "{quote}" 
        else:
            new += letter
    return new


output = []
output += [ 'left_bracket = "{"' ]
output += [ 'right_bracket = "}"' ]
output += [ 'quote = \'\"\'' ]


with open(target, "r") as data:
    content = data.read()
    content = flip_paras(content)
    for line in content.split("\n"):
        if "#" in line:
            output += [ line.replace("#", 'print(f"', 1) + '")' ]
        else:
            output += [ line ]

program = open(header).read()
program += "\n".join(output)
exec(program)



