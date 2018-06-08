#!/usr/bin/python3

def greet_1(name,msg):
   """This function greets to
   the person with the provided message"""
   print("Hello",name + ', ' + msg)

greet_1("Monica","Good morning!")

def greet_2(name, msg = "Good morning!"):
   """
   This function greets to
   the person with the
   provided message.

   If message is not provided,
   it defaults to "Good morning!"
   """

   print("Hello",name + ', ' + msg)

greet_2("Kate")
greet_2("Bruce","How do you do?")

def greet_3(*names):
   """This function greets all
   the person in the names tuple."""
   # the number of arguments is unknow
   # names is a tuple with arguments
   for name in names:
       print("Hello",name)

greet_3("Monica","Luke","Steve","John")
