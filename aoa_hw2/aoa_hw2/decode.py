from cStringIO import StringIO
import sys

def main():
    if len(sys.argv) != 4:
        print "Usage : python decode.py encoded.txt hcodes.txt decoded.txt"
        return

    f = open(sys.argv[2])
    lines = f.readlines()
    f.close()
    code_dict = {}
    for line in lines:
        [value, key] = line.strip().split()
        if value == 'space':
            value = ' '
        code_dict[key] = value



    f = open(sys.argv[1])
    lines = f.readline()
    f.close()
    l = len(lines)

    org_text = StringIO()
    i,j = 0,1
    while j <= l:
        word = lines[i:j]
        if code_dict.has_key(word):
            org_text.write(code_dict[word])
            i = j
        j = j + 1
        
    if i != l:
        print 'Error in decoding!!'

    f = open(sys.argv[3], 'w')
    f.write(org_text.getvalue())

if __name__ == "__main__":
    main()
