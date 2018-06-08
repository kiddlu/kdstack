#!/usr/bin/python3

# Use of break statement inside loop

for val in "string":
    if val == "i":
        break
    print(val)

print()

for val in "string":
    if val == "i":
        continue
    print(val)

print(type("string"))
print("The end")
