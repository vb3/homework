import sys
import os
from operator import mul
from operator import add

def create_course_dir():
    #get user inputs of term and course
    input_term = raw_input("Enter term: ")
    input_course = raw_input("Enter course: ")
    
    #build path_name, check to see if exists or create
    path_name = os.environ['HOME'] + "/" + input_term 
    if not os.path.isdir(path_name):
        print "Making " + path_name
        os.mkdir(path_name)
    path_name += "/" + input_course
    if not os.path.isdir(path_name):
        print "Making " + path_name 
        os.mkdir(path_name)
    os.chdir(path_name)
    
    #create default dirs
    def_dirs = ("assignments", "examples", "exams")
    def_dirs += ("lecture_notes", "submissions")
    for dir in def_dirs:
        if not os.path.isdir(dir):
            print "Making " + dir + " directory..."
            os.mkdir(dir)

    #make symbolic links
    course_path = "/usr/local/classes/eecs/" + input_term 
    course_path += "/" + input_course
    slinks = ("public_html", "handin")
    snames = ("website", "handin")
    if os.path.isdir(course_path):
        for slink, sname in zip(slinks, snames):
            if not os.path.lexists(sname):
                print "Making symnolic link for " + course_path + "/" + slink + " at " + sname
                os.symlink(course_path + "/" + slink, sname)
    else:
        print course_path + " does not exist. Symbolic links not made."

    print "\nDONE."

def greatest_five_digit_product():
    #get number from user, error check
    input_number = raw_input("Enter the number: ")
    if len(input_number) < 5:
        sys.exit("ERROR: Number has to be atleast 5 digits")
    
    #compute the greatest product, http://correspondence.wordpress.com/
    product = 0
    for i in range(len(input_number)-4):
        product = max(reduce(mul, [int(input_number[j]) for j in range(i,i+5)]), product)
    
    print "Greatest product of five consecutive digits: %d" % product

def name_scores():
    #try to open names.txt, error check
    try:
        f = open("names.txt")
    except:
        sys.exit("ERROR: names.txt not found.")
    
    #read in line, split and sort
    names = f.readline().replace("\n", "").replace('"', "").split(",")
    names.sort()
    f.close()

    #compute name score
    namescore = reduce(add, [(i+1) * reduce(add, [ord(c) - 64 for c in list(names[i])]) for i in range(len(names))])
    print "Name score for names.txt is %d." % namescore

def triangle_words():
    #try to open names.txt, error check
    try:
        f = open("words.txt")
    except:
        sys.exit("ERROR: words.txt not found.")
    
    #read in line, split and sort
    words = f.readline().replace("\n", "").replace('"', "").split(",")
    f.close()
    
    #computer triangle numbers
    tri_nums = [int((i/2.0) * (i+1)) for i in range(32)]

    #compute number of triangle words
    counter = 0
    print "total names: %d" % len(words)
    for i in range(len(words)):
        if reduce(add, [ord(c) - 64 for c in list(words[i])]) in tri_nums:
            counter += 1

    print "Number of triangle words: %d" % counter

def main():
    print "Assignment 2"
    print "1) Create directories in home folder for given term and course"
    print "2) Find greatest product of five consecutive digits in the 1000-digit number"
    print "3) names.txt sort alpha order, find alphabetical value for each name, multiply this value by position in list. output: total all name scores in the file"
    print "4) words.txt output: number of triangle words"
    
    #Get user input and error check
    user_input = raw_input("\nEnter your choice (1, 2, 3, 4): ") 
    if user_input not in ("1", "2", "3", "4"):
        sys.exit("ERROR: Invalid entry")
    
    #call correct function based on user input
    if user_input == "1":
        create_course_dir()
    elif user_input == "2":
        greatest_five_digit_product()
    elif user_input == "3":
        name_scores()
    elif user_input == "4":
        triangle_words()

if __name__ == '__main__':
    main()
