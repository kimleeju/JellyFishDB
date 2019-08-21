import string
import sys

### 뒤에 몇개를 떼내면 되는군.

def get_digit_num(v):
    n = 0
    while v > 0:
        n += 1
        v = v//10
    return n


def main():
    if len(sys.argv) < 2:
        print("Usage:./.py filename")
        exit()

    fname=sys.argv[1]
    infile=open(fname, "r")
    outfile=open(fname+".cnt", "w")

    ## get first and last line
    # infile.seek(0, 2)
    # fsize = infile.tell()
    # # print(fsize)
    # infile.seek(max(fsize-1024, 0))

    lines = infile.readlines()
    kmin = int(lines[1])
    kmax = int(lines[-1])

    sec_num = 100 ## section number
    sec_size = (kmax-kmin)//sec_num
    count = [0]*100

    ## 자신의 section 계산
    for key in lines:
        s = (int(key)-kmin) // sec_size
        count[s-1] += 1


    for c in count:
        outfile.write(str(c)+"\n")

if __name__ == "__main__":
    main()



